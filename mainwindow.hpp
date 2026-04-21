#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>

#include "json_parser.hpp"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class TreeWidget;

class MainWindow final : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow() override;

private slots:
    void browseJsonFile();
    void loadTreeFromCurrentPath();

private:
    void clearTree();
    void setStatusMessage(const QString& message, bool ok);

    Ui::MainWindow* ui;
    TreeWidget* m_treeWidget = nullptr;
    JsonParser m_parser;
};

#endif // MAINWINDOW_HPP
