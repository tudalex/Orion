#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTextEdit>
#include <attica/provider.h>
#include <attica/providermanager.h>

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);

signals:

public slots:
    void providersChanged();
    void onContentRecieved(Attica::BaseJob*);
private:
    Attica::Provider m_provider;
    Attica::ProviderManager m_manager;
    QTextEdit * m_textEdit;

};

#endif // MAINWINDOW_H
