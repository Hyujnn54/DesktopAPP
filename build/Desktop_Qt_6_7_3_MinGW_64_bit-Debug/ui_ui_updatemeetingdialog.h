/********************************************************************************
** Form generated from reading UI file 'ui_updatemeetingdialog.ui'
**
** Created by: Qt User Interface Compiler version 6.7.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_UI_UPDATEMEETINGDIALOG_H
#define UI_UI_UPDATEMEETINGDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDateEdit>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_UpdateMeetingDialog
{
public:
    QVBoxLayout *verticalLayout;
    QFormLayout *formLayout;
    QLabel *labelTitle;
    QLineEdit *nameLineEdit;
    QLabel *labelOrganiser;
    QLineEdit *sectorLineEdit;
    QLabel *labelParticipant;
    QLineEdit *contactLineEdit;
    QLabel *labelAgenda;
    QComboBox *emailLineEdit;
    QLabel *labelDate;
    QDateEdit *consultationDateTime;
    QLabel *labelDuration;
    QLineEdit *consultantLineEdit;
    QHBoxLayout *buttonLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *confirmButton;
    QPushButton *cancelButton;

    void setupUi(QDialog *UpdateMeetingDialog)
    {
        if (UpdateMeetingDialog->objectName().isEmpty())
            UpdateMeetingDialog->setObjectName("UpdateMeetingDialog");
        UpdateMeetingDialog->resize(400, 350);
        verticalLayout = new QVBoxLayout(UpdateMeetingDialog);
        verticalLayout->setObjectName("verticalLayout");
        formLayout = new QFormLayout();
        formLayout->setObjectName("formLayout");
        labelTitle = new QLabel(UpdateMeetingDialog);
        labelTitle->setObjectName("labelTitle");

        formLayout->setWidget(0, QFormLayout::LabelRole, labelTitle);

        nameLineEdit = new QLineEdit(UpdateMeetingDialog);
        nameLineEdit->setObjectName("nameLineEdit");

        formLayout->setWidget(0, QFormLayout::FieldRole, nameLineEdit);

        labelOrganiser = new QLabel(UpdateMeetingDialog);
        labelOrganiser->setObjectName("labelOrganiser");

        formLayout->setWidget(1, QFormLayout::LabelRole, labelOrganiser);

        sectorLineEdit = new QLineEdit(UpdateMeetingDialog);
        sectorLineEdit->setObjectName("sectorLineEdit");

        formLayout->setWidget(1, QFormLayout::FieldRole, sectorLineEdit);

        labelParticipant = new QLabel(UpdateMeetingDialog);
        labelParticipant->setObjectName("labelParticipant");

        formLayout->setWidget(2, QFormLayout::LabelRole, labelParticipant);

        contactLineEdit = new QLineEdit(UpdateMeetingDialog);
        contactLineEdit->setObjectName("contactLineEdit");

        formLayout->setWidget(2, QFormLayout::FieldRole, contactLineEdit);

        labelAgenda = new QLabel(UpdateMeetingDialog);
        labelAgenda->setObjectName("labelAgenda");

        formLayout->setWidget(3, QFormLayout::LabelRole, labelAgenda);

        emailLineEdit = new QComboBox(UpdateMeetingDialog);
        emailLineEdit->addItem(QString());
        emailLineEdit->addItem(QString());
        emailLineEdit->addItem(QString());
        emailLineEdit->setObjectName("emailLineEdit");

        formLayout->setWidget(3, QFormLayout::FieldRole, emailLineEdit);

        labelDate = new QLabel(UpdateMeetingDialog);
        labelDate->setObjectName("labelDate");

        formLayout->setWidget(4, QFormLayout::LabelRole, labelDate);

        consultationDateTime = new QDateEdit(UpdateMeetingDialog);
        consultationDateTime->setObjectName("consultationDateTime");
        consultationDateTime->setCalendarPopup(true);

        formLayout->setWidget(4, QFormLayout::FieldRole, consultationDateTime);

        labelDuration = new QLabel(UpdateMeetingDialog);
        labelDuration->setObjectName("labelDuration");

        formLayout->setWidget(5, QFormLayout::LabelRole, labelDuration);

        consultantLineEdit = new QLineEdit(UpdateMeetingDialog);
        consultantLineEdit->setObjectName("consultantLineEdit");

        formLayout->setWidget(5, QFormLayout::FieldRole, consultantLineEdit);


        verticalLayout->addLayout(formLayout);

        buttonLayout = new QHBoxLayout();
        buttonLayout->setObjectName("buttonLayout");
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        buttonLayout->addItem(horizontalSpacer);

        confirmButton = new QPushButton(UpdateMeetingDialog);
        confirmButton->setObjectName("confirmButton");

        buttonLayout->addWidget(confirmButton);

        cancelButton = new QPushButton(UpdateMeetingDialog);
        cancelButton->setObjectName("cancelButton");

        buttonLayout->addWidget(cancelButton);


        verticalLayout->addLayout(buttonLayout);


        retranslateUi(UpdateMeetingDialog);
        QObject::connect(cancelButton, &QPushButton::clicked, UpdateMeetingDialog, qOverload<>(&QDialog::reject));

        QMetaObject::connectSlotsByName(UpdateMeetingDialog);
    } // setupUi

    void retranslateUi(QDialog *UpdateMeetingDialog)
    {
        UpdateMeetingDialog->setWindowTitle(QCoreApplication::translate("UpdateMeetingDialog", "Update Meeting", nullptr));
        labelTitle->setText(QCoreApplication::translate("UpdateMeetingDialog", "Title:", nullptr));
        labelOrganiser->setText(QCoreApplication::translate("UpdateMeetingDialog", "Organiser:", nullptr));
        labelParticipant->setText(QCoreApplication::translate("UpdateMeetingDialog", "Participant:", nullptr));
        labelAgenda->setText(QCoreApplication::translate("UpdateMeetingDialog", "Agenda:", nullptr));
        emailLineEdit->setItemText(0, QCoreApplication::translate("UpdateMeetingDialog", "car advice", nullptr));
        emailLineEdit->setItemText(1, QCoreApplication::translate("UpdateMeetingDialog", "life advice", nullptr));
        emailLineEdit->setItemText(2, QCoreApplication::translate("UpdateMeetingDialog", "future advice", nullptr));

        labelDate->setText(QCoreApplication::translate("UpdateMeetingDialog", "Date:", nullptr));
        labelDuration->setText(QCoreApplication::translate("UpdateMeetingDialog", "Duration (min):", nullptr));
        confirmButton->setText(QCoreApplication::translate("UpdateMeetingDialog", "Confirm", nullptr));
        cancelButton->setText(QCoreApplication::translate("UpdateMeetingDialog", "Cancel", nullptr));
    } // retranslateUi

};

namespace Ui {
    class UpdateMeetingDialog: public Ui_UpdateMeetingDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_UI_UPDATEMEETINGDIALOG_H
