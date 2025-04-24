/********************************************************************************
** Form generated from reading UI file 'updateclientdialog.ui'
**
** Created by: Qt User Interface Compiler version 6.7.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_UPDATECLIENTDIALOG_H
#define UI_UPDATECLIENTDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDateTimeEdit>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_UpdateClientDialog
{
public:
    QVBoxLayout *verticalLayout;
    QFormLayout *formLayout;
    QLabel *label;
    QLineEdit *nameLineEdit;
    QLabel *label_2;
    QLineEdit *sectorLineEdit;
    QLabel *label_3;
    QLineEdit *contactLineEdit;
    QLabel *label_4;
    QLabel *label_5;
    QComboBox *consultantComboBox;
    QDateTimeEdit *consultationDateTime;
    QLineEdit *emailLineEdit;
    QLabel *label_email;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *confirmButton;
    QPushButton *cancelButton;

    void setupUi(QDialog *UpdateClientDialog)
    {
        if (UpdateClientDialog->objectName().isEmpty())
            UpdateClientDialog->setObjectName("UpdateClientDialog");
        UpdateClientDialog->resize(400, 300);
        verticalLayout = new QVBoxLayout(UpdateClientDialog);
        verticalLayout->setObjectName("verticalLayout");
        formLayout = new QFormLayout();
        formLayout->setObjectName("formLayout");
        label = new QLabel(UpdateClientDialog);
        label->setObjectName("label");

        formLayout->setWidget(0, QFormLayout::LabelRole, label);

        nameLineEdit = new QLineEdit(UpdateClientDialog);
        nameLineEdit->setObjectName("nameLineEdit");

        formLayout->setWidget(0, QFormLayout::FieldRole, nameLineEdit);

        label_2 = new QLabel(UpdateClientDialog);
        label_2->setObjectName("label_2");

        formLayout->setWidget(1, QFormLayout::LabelRole, label_2);

        sectorLineEdit = new QLineEdit(UpdateClientDialog);
        sectorLineEdit->setObjectName("sectorLineEdit");

        formLayout->setWidget(1, QFormLayout::FieldRole, sectorLineEdit);

        label_3 = new QLabel(UpdateClientDialog);
        label_3->setObjectName("label_3");

        formLayout->setWidget(2, QFormLayout::LabelRole, label_3);

        contactLineEdit = new QLineEdit(UpdateClientDialog);
        contactLineEdit->setObjectName("contactLineEdit");

        formLayout->setWidget(2, QFormLayout::FieldRole, contactLineEdit);

        label_4 = new QLabel(UpdateClientDialog);
        label_4->setObjectName("label_4");

        formLayout->setWidget(4, QFormLayout::LabelRole, label_4);

        label_5 = new QLabel(UpdateClientDialog);
        label_5->setObjectName("label_5");

        formLayout->setWidget(5, QFormLayout::LabelRole, label_5);

        consultantComboBox = new QComboBox(UpdateClientDialog);
        consultantComboBox->setObjectName("consultantComboBox");

        formLayout->setWidget(5, QFormLayout::FieldRole, consultantComboBox);

        consultationDateTime = new QDateTimeEdit(UpdateClientDialog);
        consultationDateTime->setObjectName("consultationDateTime");

        formLayout->setWidget(4, QFormLayout::FieldRole, consultationDateTime);

        emailLineEdit = new QLineEdit(UpdateClientDialog);
        emailLineEdit->setObjectName("emailLineEdit");

        formLayout->setWidget(3, QFormLayout::FieldRole, emailLineEdit);

        label_email = new QLabel(UpdateClientDialog);
        label_email->setObjectName("label_email");

        formLayout->setWidget(3, QFormLayout::LabelRole, label_email);


        verticalLayout->addLayout(formLayout);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        confirmButton = new QPushButton(UpdateClientDialog);
        confirmButton->setObjectName("confirmButton");

        horizontalLayout->addWidget(confirmButton);

        cancelButton = new QPushButton(UpdateClientDialog);
        cancelButton->setObjectName("cancelButton");

        horizontalLayout->addWidget(cancelButton);


        verticalLayout->addLayout(horizontalLayout);


        retranslateUi(UpdateClientDialog);

        QMetaObject::connectSlotsByName(UpdateClientDialog);
    } // setupUi

    void retranslateUi(QDialog *UpdateClientDialog)
    {
        UpdateClientDialog->setWindowTitle(QCoreApplication::translate("UpdateClientDialog", "Update Client", nullptr));
        label->setText(QCoreApplication::translate("UpdateClientDialog", "Name:", nullptr));
        label_2->setText(QCoreApplication::translate("UpdateClientDialog", "Sector:", nullptr));
        label_3->setText(QCoreApplication::translate("UpdateClientDialog", "Contact:", nullptr));
        label_4->setText(QCoreApplication::translate("UpdateClientDialog", "Date:", nullptr));
        label_5->setText(QCoreApplication::translate("UpdateClientDialog", "Consultant:", nullptr));
        label_email->setText(QCoreApplication::translate("UpdateClientDialog", "Email:", nullptr));
        confirmButton->setText(QCoreApplication::translate("UpdateClientDialog", "Confirm", nullptr));
        cancelButton->setText(QCoreApplication::translate("UpdateClientDialog", "Cancel", nullptr));
    } // retranslateUi

};

namespace Ui {
    class UpdateClientDialog: public Ui_UpdateClientDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_UPDATECLIENTDIALOG_H
