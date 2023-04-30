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
        if (m_govs[i] == USERSPACE)
            ui->tableWidget->setItem(i, 0, new QTableWidgetItem("mrpi-based (userspace)"));
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
    for (const auto &g : m_govs) {
        system(("python3 " + m_graphDirectory + "drawer.py").c_str());
        QString imgPath = QString::fromStdString(m_graphDirectory + "img/" + GovernerToString(g) + ".png");
        auto img = std::shared_ptr<QImage>(new QImage());
        img->load(imgPath);
        m_graphImages.push_back(img);
    }
}
void View::on_compareButton_clicked()
{
    m_stats.clear();
    m_graphImages.clear();
    m_benchPath = ui->benchPathText->text().toStdString();
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

               auto st = runner.run(m_benchPath);
               if (i != 0) {
                   m_stats[g].power_consumtion += st.energy / st.time;
                   m_stats[g].ipc += st.inst / st.cycles;
               }
            } else {
                BasicRunner::m_collector = std::shared_ptr<IPmuCollector> (new PmuCollector());
                BasicRunner::m_controller = std::shared_ptr<ISystemController> (new SystemController());

                BasicRunner runner;

                auto st = runner.run(m_benchPath, g);
                if (i != 0) {
                    m_stats[g].power_consumtion += st.energy / st.time;
                    m_stats[g].ipc += st.inst / st.cycles;
                }
            }
        }
    }
    for (const auto &g: m_govs) {
        m_stats[g] = {m_stats[g].power_consumtion / (m_attempts - 1),
                      m_stats[g].ipc / (m_attempts - 1),};
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