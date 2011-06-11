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

    connect(&m_manager, SIGNAL(defaultProvidersLoaded()), SLOT(providersChanged()));
    // tell it to get the default Providers
    m_manager.loadDefaultProviders();
}


void MainWindow::providersChanged()
{
    if (!m_manager.providers().isEmpty()) {
        m_provider = m_manager.providerByUrl(QUrl(QString::fromAscii("http://api.opendesktop.org/v1/")));
        if (!m_provider.isValid()) {

            return;
        }
        Attica::ItemJob<Attica::Person>* job = m_provider.requestPerson(QString::fromAscii("fregl"));
        connect(job, SIGNAL(finished(Attica::BaseJob*)), SLOT(onContentRecieved(Attica::BaseJob*)));
        job->start();
    }
}

void MainWindow::onContentRecieved(Attica::BaseJob *job)
{
    Attica::ItemJob<Attica::Person> *personJob = static_cast< Attica::ItemJob<Attica::Person> * >( job );
    if (personJob->metadata().error() == Attica::Metadata::NoError)
    {
        Attica::Person p(personJob->result());
        m_textEdit->setPlainText(p.lastName());
    }
}


