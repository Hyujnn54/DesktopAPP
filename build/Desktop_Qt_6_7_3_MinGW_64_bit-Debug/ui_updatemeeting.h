/********************************************************************************
** Form generated from reading UI file 'updatemeeting.ui'
**
** Created by: Qt User Interface Compiler version 6.7.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_UPDATEMEETING_H
#define UI_UPDATEMEETING_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
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
    QLabel *label_title;
    QLineEdit *titleLineEdit;
    QLabel *label_organiser;
    QLineEdit *organiserLineEdit;
    QLabel *label_participant;
    QLineEdit *participantLineEdit;
    QLabel *label_agenda;
    QComboBox *agendaComboBox;
    QLabel *label_duration;
    QLineEdit *durationLineEdit;
    QHBoxLayout *horizontalLayout;
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
        label_title = new QLabel(UpdateMeetingDialog);
        label_title->setObjectName("label_title");

        formLayout->setWidget(0, QFormLayout::LabelRole, label_title);

        titleLineEdit = new QLineEdit(UpdateMeetingDialog);
        titleLineEdit->setObjectName("titleLineEdit");

        formLayout->setWidget(0, QFormLayout::FieldRole, titleLineEdit);

        label_organiser = new QLabel(UpdateMeetingDialog);
        label_organiser->setObjectName("label_organiser");

        formLayout->setWidget(1, QFormLayout::LabelRole, label_organiser);

        organiserLineEdit = new QLineEdit(UpdateMeetingDialog);
        organiserLineEdit->setObjectName("organiserLineEdit");

        formLayout->setWidget(1, QFormLayout::FieldRole, organiserLineEdit);

        label_participant = new QLabel(UpdateMeetingDialog);
        label_participant->setObjectName("label_participant");

        formLayout->setWidget(2, QFormLayout::LabelRole, label_participant);

        participantLineEdit = new QLineEdit(UpdateMeetingDialog);
        participantLineEdit->setObjectName("participantLineEdit");

        formLayout->setWidget(2, QFormLayout::FieldRole, participantLineEdit);

        label_agenda = new QLabel(UpdateMeetingDialog);
        label_agenda->setObjectName("label_agenda");

        formLayout->setWidget(3, QFormLayout::LabelRole, label_agenda);

        agendaComboBox = new QComboBox(UpdateMeetingDialog);
        agendaComboBox->addItem(QString());
        agendaComboBox->addItem(QString());
        agendaComboBox->addItem(QString());
        agendaComboBox->setObjectName("agendaComboBox");

        formLayout->setWidget(3, QFormLayout::FieldRole, agendaComboBox);

        label_duration = new QLabel(UpdateMeetingDialog);
        label_duration->setObjectName("label_duration");

        formLayout->setWidget(4, QFormLayout::LabelRole, label_duration);

        durationLineEdit = new QLineEdit(UpdateMeetingDialog);
        durationLineEdit->setObjectName("durationLineEdit");

        formLayout->setWidget(4, QFormLayout::FieldRole, durationLineEdit);


        verticalLayout->addLayout(formLayout);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        confirmButton = new QPushButton(UpdateMeetingDialog);
        confirmButton->setObjectName("confirmButton");

        horizontalLayout->addWidget(confirmButton);

        cancelButton = new QPushButton(UpdateMeetingDialog);
        cancelButton->setObjectName("cancelButton");

        horizontalLayout->addWidget(cancelButton);


        verticalLayout->addLayout(horizontalLayout);


        retranslateUi(UpdateMeetingDialog);
        QObject::connect(cancelButton, &QPushButton::clicked, UpdateMeetingDialog, qOverload<>(&QDialog::reject));

        QMetaObject::connectSlotsByName(UpdateMeetingDialog);
    } // setupUi

    void retranslateUi(QDialog *UpdateMeetingDialog)
    {
        UpdateMeetingDialog->setWindowTitle(QCoreApplication::translate("UpdateMeetingDialog", "Update Meeting", nullptr));
        label_title->setText(QCoreApplication::translate("UpdateMeetingDialog", "Title:", nullptr));
        label_organiser->setText(QCoreApplication::translate("UpdateMeetingDialog", "Organiser:", nullptr));
        label_participant->setText(QCoreApplication::translate("UpdateMeetingDialog", "Participant:", nullptr));
        label_agenda->setText(QCoreApplication::translate("UpdateMeetingDialog", "Agenda:", nullptr));
        agendaComboBox->setItemText(0, QCoreApplication::translate("UpdateMeetingDialog", "car advice", nullptr));
        agendaComboBox->setItemText(1, QCoreApplication::translate("UpdateMeetingDialog", "life advice", nullptr));
        agendaComboBox->setItemText(2, QCoreApplication::translate("UpdateMeetingDialog", "future advice", nullptr));

        label_duration->setText(QCoreApplication::translate("UpdateMeetingDialog", "Duration (min):", nullptr));
        confirmButton->setText(QCoreApplication::translate("UpdateMeetingDialog", "Confirm", nullptr));
        cancelButton->setText(QCoreApplication::translate("UpdateMeetingDialog", "Cancel", nullptr));
    } // retranslateUi

};

namespace Ui {
    class UpdateMeetingDialog: public Ui_UpdateMeetingDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_UPDATEMEETING_H
