#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTextEdit>
#include <QHBoxLayout>
#include <QVBoxLayout>
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
    void category_selected();
    void onContentListRecieved(Attica::BaseJob*);
private:
    Attica::Provider m_provider;
    Attica::ProviderManager m_manager;
    QTextEdit * m_textEdit;
    QVBoxLayout *m_categories;


};

#endif // MAINWINDOW_H
