/********************************************************************************
** Form generated from reading UI file 'searchdialog.ui'
**
** Created by: Qt User Interface Compiler version 6.7.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SEARCHDIALOG_H
#define UI_SEARCHDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_SearchDialog
{
public:
    QVBoxLayout *verticalLayout;
    QLabel *lblStatus;
    QLineEdit *txtResourceName;
    QPushButton *btnSearch;

    void setupUi(QDialog *SearchDialog)
    {
        if (SearchDialog->objectName().isEmpty())
            SearchDialog->setObjectName("SearchDialog");
        SearchDialog->resize(400, 300);
        verticalLayout = new QVBoxLayout(SearchDialog);
        verticalLayout->setObjectName("verticalLayout");
        lblStatus = new QLabel(SearchDialog);
        lblStatus->setObjectName("lblStatus");

        verticalLayout->addWidget(lblStatus);

        txtResourceName = new QLineEdit(SearchDialog);
        txtResourceName->setObjectName("txtResourceName");

        verticalLayout->addWidget(txtResourceName);

        btnSearch = new QPushButton(SearchDialog);
        btnSearch->setObjectName("btnSearch");

        verticalLayout->addWidget(btnSearch);


        retranslateUi(SearchDialog);

        QMetaObject::connectSlotsByName(SearchDialog);
    } // setupUi

    void retranslateUi(QDialog *SearchDialog)
    {
        SearchDialog->setWindowTitle(QCoreApplication::translate("SearchDialog", "Search Resource", nullptr));
        lblStatus->setText(QCoreApplication::translate("SearchDialog", "Enter resource name and search...", nullptr));
        txtResourceName->setPlaceholderText(QCoreApplication::translate("SearchDialog", "Type resource name here...", nullptr));
        btnSearch->setText(QCoreApplication::translate("SearchDialog", "Search Resource", nullptr));
    } // retranslateUi

};

namespace Ui {
    class SearchDialog: public Ui_SearchDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SEARCHDIALOG_H
