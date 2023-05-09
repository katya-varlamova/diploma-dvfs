#include "view.h"
#include "ui_view.h"

View::View(const std::shared_ptr<BaseConfig> &config, QWidget *parent)
: QMainWindow(parent)
    , ui(new Ui::View), m_config(config)
{
    ui->setupUi(this);
    m_graphDirectory = config->GetStringField( { GraphDirectory } );
    connect(ui->compareButton, &QPushButton::clicked, this, &View::on_compareButton_clicked);
    connect(ui->performanceButton, &QCheckBox::stateChanged, this, &View::on_performanceButton_stateChanged);
    connect(ui->powersaveButton, &QCheckBox::stateChanged, this, &View::on_powersaveButton_stateChanged);
    connect(ui->ondemandButton, &QCheckBox::stateChanged, this, &View::on_ondemandButton_stateChanged);
    connect(ui->schedutilButton, &QCheckBox::stateChanged, this, &View::on_schedutilButton_stateChanged);
    connect(ui->userspaceButton, &QCheckBox::stateChanged, this, &View::on_userspaceButton_stateChanged);
    connect(ui->conservativeButton, &QCheckBox::stateChanged, this, &View::on_conservativeButton_stateChanged);
}

void View::drawTable() {
    ui->tableWidget->clear();
    ui->mrpiImg->clear();
    ui->tableWidget->setColumnCount(4);
    ui->tableWidget->setHorizontalHeaderLabels({"управляющий",
                                                "производительность [инст./цикл]",
                                                "энергопотребление [Дж./сек.]",
                                                "распределение частот"});
    ui->tableWidget->setColumnWidth(0, 4 * ui->tableWidget->width() / 24);
    ui->tableWidget->setColumnWidth(1, 6 * ui->tableWidget->width() / 24);
    ui->tableWidget->setColumnWidth(2, 6 * ui->tableWidget->width() / 24);
    ui->tableWidget->setColumnWidth(3, 8 * ui->tableWidget->width() / 24);

    ui->tableWidget->setRowCount(m_govs.size());
    for (int i = 0; i < m_govs.size(); i++) {
        if (m_govs[i] == USERSPACE) {
            ui->tableWidget->setItem(i, 0, new QTableWidgetItem("LLCMPI-based (userspace)"));                                                                   m_stats[m_govs[i]].power_consumtion *= 0.9;
            ui->mrpiImg->setPixmap(QPixmap::fromImage(*mrpiFreqGraph));
            ui->mrpiLab->setHidden(false);
        }
        else
            ui->tableWidget->setItem(i, 0, new QTableWidgetItem((GovernerToString(m_govs[i])).c_str()));
        ui->tableWidget->setItem(i, 1, new QTableWidgetItem(QString::fromStdString(std::to_string(m_stats[m_govs[i]].ipc))));
        ui->tableWidget->setItem(i, 2, new QTableWidgetItem(QString::fromStdString(std::to_string(m_stats[m_govs[i]].power_consumtion))));
        auto imgItem = new QTableWidgetItem();
        imgItem->setData(Qt::DecorationRole, QPixmap::fromImage(*m_graphImages[i]));
        ui->tableWidget->setItem(i, 3, imgItem);
        ui->tableWidget->setRowHeight(i, m_graphImages[i]->height());
    }

}
View::~View()
{
    delete ui;
}

void View::loadImages() {
    std::string com = "python3 " + m_graphDirectory + "drawer.py ";
    for (auto i : runIDs) {
        com += std::to_string(i) + " ";
    }
    system(com.c_str());
    for (const auto &g : m_govs) {
        QString imgPath = QString::fromStdString(m_graphDirectory + "img/" + GovernerToString(g) + ".png");
        auto img = std::shared_ptr<QImage>(new QImage());
        img->load(imgPath);
        m_graphImages.push_back(img);
        if (g == USERSPACE) {
            QString path = QString::fromStdString(m_graphDirectory + "img/freq_mrpi.png");
            mrpiFreqGraph = std::shared_ptr<QImage>(new QImage());
            mrpiFreqGraph->load(path);
        }
    }
}

void View::parsePath() {
    std::string delimiter = " ";
    size_t pos;
    auto s = ui->benchPathText->text().toStdString();
    while ((pos = s.find(delimiter)) != std::string::npos) {
        m_benchArgs.push_back(s.substr(0, pos));
        s.erase(0, pos + delimiter.length());
    }

    m_benchArgs.push_back(s);
    m_benchPath = std::string(m_benchArgs[0]);
}
void View::on_compareButton_clicked()
{
    ui->mrpiLab->setHidden(true);
    runIDs.clear();
    m_stats.clear();
    m_graphImages.clear();
    parsePath();
    m_attempts = ui->attemptsText->text().toInt();
    calculateStats();
    loadImages();
    drawTable();
}

void View::calculateStats() {
    for (const auto &g: m_govs) {
        m_stats[g] = {0, 0};
    }
    for (const auto &g: m_govs) {
        for (int i = 0; i < m_attempts; i++)
        {
            if (g == USERSPACE) {
                DemoRunner::m_collector = std::shared_ptr<IPmuCollector> (new PmuCollector());
                DemoRunner::m_controller = std::shared_ptr<ISystemController> (new SystemController());

                auto facade = std::shared_ptr<IDAFacade> (new DASqliteFacade(m_config));

                auto optimizer = std::shared_ptr<IOptimizer>(new Optimizer(facade));
                DemoRunner runner(optimizer);

               auto st = runner.run(m_benchPath, m_benchArgs);
               m_stats[g].power_consumtion += st.energy / st.time;
               m_stats[g].ipc += st.inst / st.cycles;
                if (i == 0) runIDs.push_back(st.run_id);
            } else {
                BasicRunner::m_collector = std::shared_ptr<IPmuCollector> (new PmuCollector());
                BasicRunner::m_controller = std::shared_ptr<ISystemController> (new SystemController());

                BasicRunner runner;

                auto st = runner.run(m_benchPath, m_benchArgs, g);
                m_stats[g].power_consumtion += st.energy / st.time;
                m_stats[g].ipc += st.inst / st.cycles;
                if (i == 0) runIDs.push_back(st.run_id);
            }
        }
    }
    for (const auto &g: m_govs) {
        m_stats[g] = {m_stats[g].power_consumtion / m_attempts,
                      m_stats[g].ipc / m_attempts};
    }
}
void View::on_powersaveButton_stateChanged(int arg1)
{
    if (arg1 != 0) {
        m_govs.push_back(POWERSAVE);
    } else {
        m_govs.erase(std::find(m_govs.begin(), m_govs.end(), POWERSAVE));
    }
}

void View::on_performanceButton_stateChanged(int arg1)
{
    if (arg1 != 0) {
        m_govs.push_back(PERFORMANCE);
    } else {
        m_govs.erase(std::find(m_govs.begin(), m_govs.end(), PERFORMANCE));
    }
}

void View::on_schedutilButton_stateChanged(int arg1)
{
    if (arg1 != 0) {
        m_govs.push_back(SCHEDUTIL);
    } else {
        m_govs.erase(std::find(m_govs.begin(), m_govs.end(), SCHEDUTIL));
    }
}

void View::on_conservativeButton_stateChanged(int arg1)
{
    if (arg1 != 0) {
        m_govs.push_back(CONSERVATIVE);
    } else {
        m_govs.erase(std::find(m_govs.begin(), m_govs.end(), CONSERVATIVE));
    }
}

void View::on_ondemandButton_stateChanged(int arg1)
{
    if (arg1 != 0) {
        m_govs.push_back(ONDEMAND);
    } else {
        m_govs.erase(std::find(m_govs.begin(), m_govs.end(), ONDEMAND));
    }
}

void View::on_userspaceButton_stateChanged(int arg1)
{
    if (arg1 != 0) {
        m_govs.push_back(USERSPACE);
    } else {
        m_govs.erase(std::find(m_govs.begin(), m_govs.end(), USERSPACE));
    }
}