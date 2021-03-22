#include "comboboxdelegate.h"
#include <QComboBox>
#include <QtDebug>
#include <QApplication>
#include <QStyle>

#pragma warning(disable:4100)
ComboBoxDelegate::ComboBoxDelegate(QObject *parent)
    :QStyledItemDelegate(parent)
{
    qDebug()<<"delegate constructor";
}

ComboBoxDelegate::~ComboBoxDelegate()
{
    qDebug()<<"delegate destructor";
}

QWidget *ComboBoxDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QComboBox *editor = new QComboBox(parent);

    QHash<QString, QVariant> itemData = index.model()->data(index, Qt::UserRole+2).toHash();
    auto iter = itemData.cbegin();
    while(iter != itemData.cend() ) {
        editor->addItem( iter.key(), iter.value() );
        ++iter;
    }

    editor->setCurrentIndex(0);
    return editor;
}

void ComboBoxDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QComboBox *cb = static_cast<QComboBox*>(editor);
    QString value = index.model()->data(index, Qt::DisplayRole).toString();
    int data = index.model()->data(index, Qt::UserRole + 1).toInt();

    qDebug()<<"text="<<value<<",data="<<data;

    int idx = cb->findData(data);
    if (idx > -1 ) {
        cb->setCurrentIndex(idx);
    }
}

void ComboBoxDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QComboBox *cb = static_cast<QComboBox*>(editor);
    QString displayData = cb->currentText();
    QVariant value = cb->currentData();
    model->setData(index, displayData, Qt::DisplayRole);
    model->setData( index, value, Qt::UserRole + 1 );
}

void ComboBoxDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    editor->setGeometry(option.rect);
}
