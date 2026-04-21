#include "mainwindow.hpp"

#include "tree_widget.hpp"
#include "ui_mainwindow.h"

#include <QFile>
#include <QFileDialog>
#include <QFileInfo>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QScrollArea>

#include <memory>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_treeWidget(new TreeWidget)
{
    ui->setupUi(this);

    ui->treeScrollArea->setWidget(m_treeWidget);
    ui->treeScrollArea->setWidgetResizable(false);

    connect(ui->browseButton, &QPushButton::clicked, this, &MainWindow::browseJsonFile);
    connect(ui->loadButton, &QPushButton::clicked, this, &MainWindow::loadTreeFromCurrentPath);
    connect(ui->filePathEdit, &QLineEdit::returnPressed, this, &MainWindow::loadTreeFromCurrentPath);

    setStatusMessage(QStringLiteral("Choose a JSON file to load the tree."), true);
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

    if (filePath.isEmpty()) {
        return;
    }

    ui->filePathEdit->setText(filePath);
    loadTreeFromCurrentPath();
}

void MainWindow::loadTreeFromCurrentPath()
{
    const QString filePath = ui->filePathEdit->text().trimmed();
    if (filePath.isEmpty()) {
        clearTree();
        setStatusMessage(QStringLiteral("Specify a JSON file path."), false);
        return;
    }

    QFile inputFile(filePath);
    if (!inputFile.exists()) {
        clearTree();
        setStatusMessage(QStringLiteral("File not found: %1").arg(filePath), false);
        return;
    }

    if (!inputFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        clearTree();
        setStatusMessage(
            QStringLiteral("Cannot open file: %1").arg(inputFile.errorString()),
            false);
        return;
    }

    const QByteArray rawJson = inputFile.readAll();
    inputFile.close();

    QJsonParseError parseError;
    const QJsonDocument document = QJsonDocument::fromJson(rawJson, &parseError);
    if (parseError.error != QJsonParseError::NoError) {
        clearTree();
        setStatusMessage(
            QStringLiteral("JSON parse error in %1: %2")
                .arg(QFileInfo(inputFile).fileName(), parseError.errorString()),
            false);
        return;
    }

    auto result = m_parser.parseJson(document);
    if (!result.ok) {
        clearTree();
        setStatusMessage(result.message, false);
        return;
    }

    auto root = std::make_unique<BinaryTree::Node>(std::move(result.value));
    const QString summary = QStringLiteral(
        "Loaded %1: %2 node(s), height %3")
                                .arg(QFileInfo(inputFile).fileName())
                                .arg(root->size())
                                .arg(root->height());

    m_treeWidget->setTree(std::move(root));
    m_treeWidget->adjustSize();
    setStatusMessage(summary, true);
}

void MainWindow::clearTree()
{
    m_treeWidget->setTree(nullptr);
    m_treeWidget->adjustSize();
}

void MainWindow::setStatusMessage(const QString& message, bool ok)
{
    ui->statusLabel->setText(message);
    ui->statusLabel->setStyleSheet(ok
            ? QStringLiteral("color: #166534;")
            : QStringLiteral("color: #B91C1C;"));
}
