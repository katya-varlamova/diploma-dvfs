#pragma once
#include <QMainWindow>
#include "ui_view.h"
#include "core/PmuCollector.h"
#include "core/SystemController.h"
#include "workloads/IdleWorkload.h"
#include "workloads/PortionWorkload.h"
#include "DataCollection/Collector.h"
#include "Runner/DemoRunner.h"
#include "Runner/Optimizer.h"
#include "Sqlite/DASqliteFacade.h"
#include "config/yamlcpp_config.h"
#include "Runner/BasicRunner.h"

QT_BEGIN_NAMESPACE
namespace Ui { class View; }
QT_END_NAMESPACE
typedef struct {
    double power_consumtion;
    double ipc;
} result_stats_t;
class View : public QMainWindow
{

public:
    View(const std::shared_ptr<BaseConfig> &config, QWidget *parent = nullptr);
    ~View();

private slots:
    void on_powersaveButton_stateChanged(int arg1);

    void on_performanceButton_stateChanged(int arg1);

    void on_schedutilButton_stateChanged(int arg1);

    void on_conservativeButton_stateChanged(int arg1);

    void on_ondemandButton_stateChanged(int arg1);

    void on_userspaceButton_stateChanged(int arg1);

    void on_compareButton_clicked();

    void drawTable();

    void calculateStats();

    void loadImages();

private:
    Ui::View *ui;
    std::vector<governor_t> m_govs;
    std::string m_benchPath;
    int m_attempts;
    std::shared_ptr<BaseConfig> m_config;
    std::map<governor_t, result_stats_t> m_stats;
    std::string m_graphDirectory;
    std::vector<std::shared_ptr<QImage>> m_graphImages;
    std::shared_ptr<QImage> mrpiFreqGraph;
    std::vector<int> runIDs;
};
