#include "softwareitem.h"

SoftwareItem::SoftwareItem( Attica::Content *content)
{
    this->setText(content->name());
    //this->setIcon(QIcon::addFile(content->icons()));
    this->m_content = content;
}
Attica::Content * SoftwareItem::getContent()
{
    return m_content;
}
