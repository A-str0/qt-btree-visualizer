#include "mainwindow.hpp"

#include "tree_widget.hpp"
#include "ui_mainwindow.h"

#include <QFile>
#include <QFileDialog>
#include <QVBoxLayout>
#include <QFileInfo>
#include <QJsonDocument>
#include <QJsonParseError>

#include <memory>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_treeWidget(new TreeWidget)
{
    ui->setupUi(this);

    auto* containerLayout = new QVBoxLayout(ui->treeContainer);
    containerLayout->setContentsMargins(0, 0, 0, 0);
    containerLayout->addWidget(m_treeWidget);

    connect(ui->browseButton, &QPushButton::clicked, this, &MainWindow::browseJsonFile);
    connect(ui->filePathEdit, &QLineEdit::returnPressed, this, &MainWindow::loadTreeFromCurrentPath);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::browseJsonFile()
{
    const QString filePath = QFileDialog::getOpenFileName(
        this,
        QStringLiteral("Open JSON file"),
        ui->filePathEdit->text(),
        QStringLiteral("JSON Files (*.json);;All Files (*)"));

    if (filePath.isEmpty())
        return;

    ui->filePathEdit->setText(filePath);
    loadTreeFromCurrentPath();
}

void MainWindow::loadTreeFromCurrentPath()
{
    const QString filePath = ui->filePathEdit->text().trimmed();
    if (filePath.isEmpty()) {
        clearTree();

        return;
    }

    QFile inputFile(filePath);
    if (!inputFile.exists()) {
        clearTree();

        return;
    }

    if (!inputFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        clearTree();

        return;
    }

    const QByteArray rawJson = inputFile.readAll();
    inputFile.close();

    QJsonParseError parseError;
    const QJsonDocument document = QJsonDocument::fromJson(rawJson, &parseError);
    if (parseError.error != QJsonParseError::NoError) {
        clearTree();

        return;
    }

    auto result = m_parser.parseJson(document);
    if (!result.ok) {
        clearTree();

        return;
    }

    auto root = std::make_unique<BinaryTree::Node>(std::move(result.value));

    m_treeWidget->setTree(std::move(root));
}

void MainWindow::clearTree()
{
    m_treeWidget->setTree(nullptr);
}
