#include "view.h"

#include <QApplication>
int IRunner::run_id = 0;
int main(int argc, char *argv[])
{
    auto config = std::shared_ptr<BaseConfig> (new YamlCppConfig("/home/kate/Desktop/diploma/repo/Config/config.txt"));
    LoggerFactory::InitLogger(config);
    QApplication a(argc, argv);
    View w(config, nullptr);
    w.show();
    return a.exec();
}
