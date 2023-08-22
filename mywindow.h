#ifndef MYWINDOW_H
#define MYWINDOW_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QGridLayout>
#include <QObject>
#include <QDateTime>

class MyWindow : public QWidget
{
    Q_OBJECT

public:
    explicit MyWindow(QWidget *parent = nullptr);
    struct records {
        int attempts;
        QString dateTime;
    };

private:
    QLabel *statLabel{};
    QLabel *titleLabel{};
    QLabel *infoLabel{};
    QLineEdit *numberInput{};
    QLabel *resultLabel{};
    QPushButton *submitBtn{};
    QPushButton *restartBtn{};
    QVector<int> secretNumber;
    int attempts = 0;
    QString statText = QString("Statistics:\n");

    void setupUI();
    void generateSecretNumber();
    void checkNumber();
    void restartGame();
    void changeStat(std::vector<records>& records_list);
    void saveRecord(int attempts);
    static bool compareByAttempts(const records& a, const records& b);
    void getRecord();
};

#endif // MYWINDOW_H
