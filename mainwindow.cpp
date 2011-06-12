#include "mainwindow.h"
#include <attica/provider.h>
#include <attica/providermanager.h>
#include <attica/person.h>
#include <attica/itemjob.h>
#include <QTextEdit>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    m_textEdit = new QTextEdit();
    m_textEdit->setPlainText(QString::fromAscii("test"));
    this->setCentralWidget(m_textEdit);

    connect(&m_manager, SIGNAL(defaultProvidersLoaded()), SLOT(providersChanged()));
    // tell it to get the default Providers
    m_manager.loadDefaultProviders();
}


void MainWindow::providersChanged()
{
    qDebug("provider has changed");
    if (!m_manager.providers().isEmpty()) {
        qDebug("provider list is not empty");
        m_provider = m_manager.providerByUrl(QUrl(QString::fromAscii("https://api.opendesktop.org/v1/")));
        if (!m_provider.isValid()) {
            qDebug("provider is not valid");
            return;
        }
        Attica::ItemJob<Attica::Person>* job = m_provider.requestPerson(QString::fromAscii("fregl"));
        connect(job, SIGNAL(finished(Attica::BaseJob*)), SLOT(onContentRecieved(Attica::BaseJob*)));
        job->start();
    }

}

void MainWindow::onContentRecieved(Attica::BaseJob *job)
{
    qDebug("Job finished");
    Attica::ItemJob<Attica::Person> *personJob = static_cast< Attica::ItemJob<Attica::Person> * >( job );
    if (personJob->metadata().error() == Attica::Metadata::NoError)
    {
        Attica::Person p(personJob->result());
        m_textEdit->setPlainText(p.lastName());
    }
    else
        qDebug("job didn't go well");
}


