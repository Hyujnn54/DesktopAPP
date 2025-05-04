/********************************************************************************
** Form generated from reading UI file 'updatetrainingdialog.ui'
**
** Created by: Qt User Interface Compiler version 6.7.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_UPDATETRAININGDIALOG_H
#define UI_UPDATETRAININGDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDateEdit>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_UpdateTrainingDialog
{
public:
    QVBoxLayout *verticalLayout;
    QFormLayout *formLayout;
    QLabel *label_formation;
    QLineEdit *formationLineEdit;
    QLabel *label_description;
    QLineEdit *descriptionLineEdit;
    QLabel *label_trainer;
    QLineEdit *trainerLineEdit;
    QLabel *label_date;
    QDateEdit *dateEdit;
    QLabel *label_time;
    QSpinBox *timeSpinBox;
    QLabel *label_prix;
    QDoubleSpinBox *prixSpinBox;
    QGroupBox *resourcesGroupBox;
    QVBoxLayout *resourcesVBoxLayout;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *confirmButton;
    QPushButton *cancelButton;

    void setupUi(QDialog *UpdateTrainingDialog)
    {
        if (UpdateTrainingDialog->objectName().isEmpty())
            UpdateTrainingDialog->setObjectName("UpdateTrainingDialog");
        UpdateTrainingDialog->resize(400, 300);
        verticalLayout = new QVBoxLayout(UpdateTrainingDialog);
        verticalLayout->setObjectName("verticalLayout");
        formLayout = new QFormLayout();
        formLayout->setObjectName("formLayout");
        label_formation = new QLabel(UpdateTrainingDialog);
        label_formation->setObjectName("label_formation");

        formLayout->setWidget(0, QFormLayout::LabelRole, label_formation);

        formationLineEdit = new QLineEdit(UpdateTrainingDialog);
        formationLineEdit->setObjectName("formationLineEdit");

        formLayout->setWidget(0, QFormLayout::FieldRole, formationLineEdit);

        label_description = new QLabel(UpdateTrainingDialog);
        label_description->setObjectName("label_description");

        formLayout->setWidget(1, QFormLayout::LabelRole, label_description);

        descriptionLineEdit = new QLineEdit(UpdateTrainingDialog);
        descriptionLineEdit->setObjectName("descriptionLineEdit");

        formLayout->setWidget(1, QFormLayout::FieldRole, descriptionLineEdit);

        label_trainer = new QLabel(UpdateTrainingDialog);
        label_trainer->setObjectName("label_trainer");

        formLayout->setWidget(2, QFormLayout::LabelRole, label_trainer);

        trainerLineEdit = new QLineEdit(UpdateTrainingDialog);
        trainerLineEdit->setObjectName("trainerLineEdit");

        formLayout->setWidget(2, QFormLayout::FieldRole, trainerLineEdit);

        label_date = new QLabel(UpdateTrainingDialog);
        label_date->setObjectName("label_date");

        formLayout->setWidget(3, QFormLayout::LabelRole, label_date);

        dateEdit = new QDateEdit(UpdateTrainingDialog);
        dateEdit->setObjectName("dateEdit");
        dateEdit->setCalendarPopup(true);

        formLayout->setWidget(3, QFormLayout::FieldRole, dateEdit);

        label_time = new QLabel(UpdateTrainingDialog);
        label_time->setObjectName("label_time");

        formLayout->setWidget(4, QFormLayout::LabelRole, label_time);

        timeSpinBox = new QSpinBox(UpdateTrainingDialog);
        timeSpinBox->setObjectName("timeSpinBox");
        timeSpinBox->setMinimum(1);
        timeSpinBox->setMaximum(30);

        formLayout->setWidget(4, QFormLayout::FieldRole, timeSpinBox);

        label_prix = new QLabel(UpdateTrainingDialog);
        label_prix->setObjectName("label_prix");

        formLayout->setWidget(5, QFormLayout::LabelRole, label_prix);

        prixSpinBox = new QDoubleSpinBox(UpdateTrainingDialog);
        prixSpinBox->setObjectName("prixSpinBox");
        prixSpinBox->setMinimum(1.000000000000000);
        prixSpinBox->setMaximum(1000.000000000000000);
        prixSpinBox->setDecimals(2);

        formLayout->setWidget(5, QFormLayout::FieldRole, prixSpinBox);


        verticalLayout->addLayout(formLayout);

        resourcesGroupBox = new QGroupBox(UpdateTrainingDialog);
        resourcesGroupBox->setObjectName("resourcesGroupBox");
        resourcesVBoxLayout = new QVBoxLayout(resourcesGroupBox);
        resourcesVBoxLayout->setObjectName("resourcesVBoxLayout");

        verticalLayout->addWidget(resourcesGroupBox);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        confirmButton = new QPushButton(UpdateTrainingDialog);
        confirmButton->setObjectName("confirmButton");

        horizontalLayout->addWidget(confirmButton);

        cancelButton = new QPushButton(UpdateTrainingDialog);
        cancelButton->setObjectName("cancelButton");

        horizontalLayout->addWidget(cancelButton);


        verticalLayout->addLayout(horizontalLayout);


        retranslateUi(UpdateTrainingDialog);

        QMetaObject::connectSlotsByName(UpdateTrainingDialog);
    } // setupUi

    void retranslateUi(QDialog *UpdateTrainingDialog)
    {
        UpdateTrainingDialog->setWindowTitle(QCoreApplication::translate("UpdateTrainingDialog", "Update Training", nullptr));
        label_formation->setText(QCoreApplication::translate("UpdateTrainingDialog", "Formation:", nullptr));
        label_description->setText(QCoreApplication::translate("UpdateTrainingDialog", "Description:", nullptr));
        label_trainer->setText(QCoreApplication::translate("UpdateTrainingDialog", "Trainer:", nullptr));
        label_date->setText(QCoreApplication::translate("UpdateTrainingDialog", "Date:", nullptr));
        label_time->setText(QCoreApplication::translate("UpdateTrainingDialog", "Time (hours):", nullptr));
        label_prix->setText(QCoreApplication::translate("UpdateTrainingDialog", "Prix:", nullptr));
        resourcesGroupBox->setTitle(QCoreApplication::translate("UpdateTrainingDialog", "Resources Used", nullptr));
        confirmButton->setText(QCoreApplication::translate("UpdateTrainingDialog", "Confirm", nullptr));
        cancelButton->setText(QCoreApplication::translate("UpdateTrainingDialog", "Cancel", nullptr));
    } // retranslateUi

};

namespace Ui {
    class UpdateTrainingDialog: public Ui_UpdateTrainingDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_UPDATETRAININGDIALOG_H
