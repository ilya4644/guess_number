#include "mywindow.h"
#include <QMessageBox>
#include <QTextStream>
#include <QDateTime>
#include <QRandomGenerator>

MyWindow::MyWindow(QWidget *parent) : QWidget(parent)
{
    setupUI();
    generateSecretNumber();
}

void MyWindow::setupUI()
{
    statLabel = new QLabel("Statistics:\n", this);
    statLabel->setAlignment(Qt::AlignCenter);

    titleLabel = new QLabel("Guess the number", this);
    titleLabel->setAlignment(Qt::AlignCenter);

    infoLabel = new QLabel("Enter a four-digit number:", this);
    infoLabel->setAlignment(Qt::AlignCenter);

    numberInput = new QLineEdit(this);
    connect(numberInput, &QLineEdit::returnPressed, this, &MyWindow::checkNumber);

    resultLabel = new QLabel(this);
    resultLabel->setAlignment(Qt::AlignCenter);

    submitBtn = new QPushButton("Check", this);
    connect(submitBtn, &QPushButton::clicked, this, &MyWindow::checkNumber);

    restartBtn = new QPushButton("Start over", this);
    connect(restartBtn, &QPushButton::clicked, this, &MyWindow::restartGame);

    auto *mainLayout = new QGridLayout(this);
    mainLayout->addWidget(statLabel, 0, 0, 1, 2);
    mainLayout->addWidget(titleLabel, 1, 0, 1, 2);
    mainLayout->addWidget(infoLabel, 2, 0, 1, 2);
    mainLayout->addWidget(numberInput, 3, 0, 1, 2);
    mainLayout->addWidget(resultLabel, 4, 0, 1, 2);
    mainLayout->addWidget(submitBtn, 5, 0);
    mainLayout->addWidget(restartBtn, 5, 1);

    setLayout(mainLayout);
    setWindowTitle("Guess the number");
    setFixedSize(500, 400);
}

void MyWindow::generateSecretNumber()
{
    secretNumber.clear();

    while (secretNumber.size() < 4)
    {
        int digit = QRandomGenerator::global()->bounded(10);
        if (!secretNumber.contains(digit))
            secretNumber.append(digit);
    }
}

void MyWindow::checkNumber()
{
    QString inputText = numberInput->text();
    if (inputText.length() != 4 || !inputText.toInt())
    {
        QMessageBox::warning(this, "Error", "Please enter a four-digit number.");
        return;
    }

    QVector<int> inputNumber;
    for (int i = 0; i < 4; ++i)
        inputNumber.append(inputText.at(i).digitValue());

    int correctDigits = 0;
    int correctPositions = 0;

    for (int i = 0; i < 4; ++i)
    {
        if (inputNumber.at(i) == secretNumber.at(i))
            correctPositions++;
        else if (secretNumber.contains(inputNumber.at(i)))
            correctDigits++;
    }

    attempts++;

    QString resultText = QString("Attempt %1: %2 the numbers are in their places, %3 guessed numbers")
            .arg(attempts)
            .arg(correctPositions)
            .arg(correctDigits);

    resultLabel->setText(resultText);

    if (correctPositions == 4)
    {
        QString message = QString("Congratulations, you guessed the number in %1 attempts!").arg(attempts);
        QMessageBox::information(this, "Win!", message);
        saveRecord(attempts);
        restartGame();
    }

    numberInput->clear();
    numberInput->setFocus();
}

void MyWindow::restartGame()
{
    generateSecretNumber();
    attempts = 0;
    resultLabel->clear();
    getRecord();
}

void MyWindow::changeStat(std::vector<records>& records_list)
{
    statText = QString("Statistics:\n");
    for (const auto& item : records_list) {
        statText += QString("Time: %1: %2 attempts\n")
                .arg((item.dateTime.size() >= 0) ? item.dateTime : "not save")
                .arg(item.attempts);
    }
    statLabel->setText(statText);
}

void MyWindow::saveRecord(int attempts)
{
    records record = {attempts, QDateTime::currentDateTime().toString("dd.MM.yyyy")};
    FILE* file = nullptr;
    errno_t err = fopen_s(&file, "records.bin", "ab");
    if (err == 0) {
        fwrite(&record, sizeof(record), 1, file);
    }
    fclose(file);
}

bool MyWindow::compareByAttempts(const records& a, const records& b) {
    return a.attempts < b.attempts;
}

void MyWindow::getRecord()
{
    FILE* file = nullptr;
    fopen_s(&file, "records.bin", "rb");
    std::vector<records> records_list;
    records record;
    if (file != nullptr) {
        while (fread(&record, sizeof(record), 1, file) == 1) {
            records_list.push_back(record);
        }
        std::sort(records_list.begin(), records_list.end(), compareByAttempts);
        if (records_list.size() > 10) {
            records_list.resize(10);
        }
        changeStat(records_list);
    }
}
