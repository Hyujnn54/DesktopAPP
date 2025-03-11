/********************************************************************************
** Form generated from reading UI file 'UpdateClientDialog.ui'
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
#include <QtWidgets/QDateEdit>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_UpdateEmployeeDialog
{
public:
    QVBoxLayout *verticalLayout;
    QFormLayout *formLayout;
    QLabel *label_cin;
    QLineEdit *cinLineEdit;
    QLabel *label_lastName;
    QLineEdit *lastNameLineEdit;
    QLabel *label_firstName;
    QLineEdit *firstNameLineEdit;
    QLabel *label_dateOfBirth;
    QDateEdit *dateOfBirthEdit;
    QLabel *label_phone;
    QLineEdit *phoneLineEdit;
    QLabel *label_email;
    QLineEdit *emailLineEdit;
    QLabel *label_gender;
    QHBoxLayout *genderLayout;
    QRadioButton *radioButton_H;
    QRadioButton *radioButton_F;
    QLabel *label_salary;
    QLineEdit *salaryLineEdit;
    QLabel *label_dateOfHire;
    QDateEdit *dateOfHireEdit;
    QLabel *label_specialty;
    QComboBox *specialtyComboBox;
    QLabel *label_image;
    QHBoxLayout *imageLayout;
    QLabel *imageLabel;
    QPushButton *selectImageButton;
    QLabel *label_role;
    QComboBox *roleComboBox;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *confirmButton;
    QPushButton *cancelButton;

    void setupUi(QDialog *UpdateEmployeeDialog)
    {
        if (UpdateEmployeeDialog->objectName().isEmpty())
            UpdateEmployeeDialog->setObjectName("UpdateEmployeeDialog");
        UpdateEmployeeDialog->resize(400, 600);
        verticalLayout = new QVBoxLayout(UpdateEmployeeDialog);
        verticalLayout->setObjectName("verticalLayout");
        formLayout = new QFormLayout();
        formLayout->setObjectName("formLayout");
        label_cin = new QLabel(UpdateEmployeeDialog);
        label_cin->setObjectName("label_cin");

        formLayout->setWidget(0, QFormLayout::LabelRole, label_cin);

        cinLineEdit = new QLineEdit(UpdateEmployeeDialog);
        cinLineEdit->setObjectName("cinLineEdit");

        formLayout->setWidget(0, QFormLayout::FieldRole, cinLineEdit);

        label_lastName = new QLabel(UpdateEmployeeDialog);
        label_lastName->setObjectName("label_lastName");

        formLayout->setWidget(1, QFormLayout::LabelRole, label_lastName);

        lastNameLineEdit = new QLineEdit(UpdateEmployeeDialog);
        lastNameLineEdit->setObjectName("lastNameLineEdit");

        formLayout->setWidget(1, QFormLayout::FieldRole, lastNameLineEdit);

        label_firstName = new QLabel(UpdateEmployeeDialog);
        label_firstName->setObjectName("label_firstName");

        formLayout->setWidget(2, QFormLayout::LabelRole, label_firstName);

        firstNameLineEdit = new QLineEdit(UpdateEmployeeDialog);
        firstNameLineEdit->setObjectName("firstNameLineEdit");

        formLayout->setWidget(2, QFormLayout::FieldRole, firstNameLineEdit);

        label_dateOfBirth = new QLabel(UpdateEmployeeDialog);
        label_dateOfBirth->setObjectName("label_dateOfBirth");

        formLayout->setWidget(3, QFormLayout::LabelRole, label_dateOfBirth);

        dateOfBirthEdit = new QDateEdit(UpdateEmployeeDialog);
        dateOfBirthEdit->setObjectName("dateOfBirthEdit");

        formLayout->setWidget(3, QFormLayout::FieldRole, dateOfBirthEdit);

        label_phone = new QLabel(UpdateEmployeeDialog);
        label_phone->setObjectName("label_phone");

        formLayout->setWidget(4, QFormLayout::LabelRole, label_phone);

        phoneLineEdit = new QLineEdit(UpdateEmployeeDialog);
        phoneLineEdit->setObjectName("phoneLineEdit");

        formLayout->setWidget(4, QFormLayout::FieldRole, phoneLineEdit);

        label_email = new QLabel(UpdateEmployeeDialog);
        label_email->setObjectName("label_email");

        formLayout->setWidget(5, QFormLayout::LabelRole, label_email);

        emailLineEdit = new QLineEdit(UpdateEmployeeDialog);
        emailLineEdit->setObjectName("emailLineEdit");

        formLayout->setWidget(5, QFormLayout::FieldRole, emailLineEdit);

        label_gender = new QLabel(UpdateEmployeeDialog);
        label_gender->setObjectName("label_gender");

        formLayout->setWidget(6, QFormLayout::LabelRole, label_gender);

        genderLayout = new QHBoxLayout();
        genderLayout->setObjectName("genderLayout");
        radioButton_H = new QRadioButton(UpdateEmployeeDialog);
        radioButton_H->setObjectName("radioButton_H");

        genderLayout->addWidget(radioButton_H);

        radioButton_F = new QRadioButton(UpdateEmployeeDialog);
        radioButton_F->setObjectName("radioButton_F");

        genderLayout->addWidget(radioButton_F);


        formLayout->setLayout(6, QFormLayout::FieldRole, genderLayout);

        label_salary = new QLabel(UpdateEmployeeDialog);
        label_salary->setObjectName("label_salary");

        formLayout->setWidget(7, QFormLayout::LabelRole, label_salary);

        salaryLineEdit = new QLineEdit(UpdateEmployeeDialog);
        salaryLineEdit->setObjectName("salaryLineEdit");

        formLayout->setWidget(7, QFormLayout::FieldRole, salaryLineEdit);

        label_dateOfHire = new QLabel(UpdateEmployeeDialog);
        label_dateOfHire->setObjectName("label_dateOfHire");

        formLayout->setWidget(8, QFormLayout::LabelRole, label_dateOfHire);

        dateOfHireEdit = new QDateEdit(UpdateEmployeeDialog);
        dateOfHireEdit->setObjectName("dateOfHireEdit");

        formLayout->setWidget(8, QFormLayout::FieldRole, dateOfHireEdit);

        label_specialty = new QLabel(UpdateEmployeeDialog);
        label_specialty->setObjectName("label_specialty");

        formLayout->setWidget(9, QFormLayout::LabelRole, label_specialty);

        specialtyComboBox = new QComboBox(UpdateEmployeeDialog);
        specialtyComboBox->addItem(QString());
        specialtyComboBox->addItem(QString());
        specialtyComboBox->addItem(QString());
        specialtyComboBox->setObjectName("specialtyComboBox");

        formLayout->setWidget(9, QFormLayout::FieldRole, specialtyComboBox);

        label_image = new QLabel(UpdateEmployeeDialog);
        label_image->setObjectName("label_image");

        formLayout->setWidget(10, QFormLayout::LabelRole, label_image);

        imageLayout = new QHBoxLayout();
        imageLayout->setObjectName("imageLayout");
        imageLabel = new QLabel(UpdateEmployeeDialog);
        imageLabel->setObjectName("imageLabel");
        imageLabel->setMinimumSize(QSize(100, 100));
        imageLabel->setFrameShape(QFrame::Box);
        imageLabel->setAlignment(Qt::AlignCenter);

        imageLayout->addWidget(imageLabel);

        selectImageButton = new QPushButton(UpdateEmployeeDialog);
        selectImageButton->setObjectName("selectImageButton");

        imageLayout->addWidget(selectImageButton);


        formLayout->setLayout(10, QFormLayout::FieldRole, imageLayout);

        label_role = new QLabel(UpdateEmployeeDialog);
        label_role->setObjectName("label_role");

        formLayout->setWidget(11, QFormLayout::LabelRole, label_role);

        roleComboBox = new QComboBox(UpdateEmployeeDialog);
        roleComboBox->addItem(QString());
        roleComboBox->addItem(QString());
        roleComboBox->addItem(QString());
        roleComboBox->setObjectName("roleComboBox");

        formLayout->setWidget(11, QFormLayout::FieldRole, roleComboBox);


        verticalLayout->addLayout(formLayout);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        confirmButton = new QPushButton(UpdateEmployeeDialog);
        confirmButton->setObjectName("confirmButton");

        horizontalLayout->addWidget(confirmButton);

        cancelButton = new QPushButton(UpdateEmployeeDialog);
        cancelButton->setObjectName("cancelButton");

        horizontalLayout->addWidget(cancelButton);


        verticalLayout->addLayout(horizontalLayout);


        retranslateUi(UpdateEmployeeDialog);

        QMetaObject::connectSlotsByName(UpdateEmployeeDialog);
    } // setupUi

    void retranslateUi(QDialog *UpdateEmployeeDialog)
    {
        UpdateEmployeeDialog->setWindowTitle(QCoreApplication::translate("UpdateEmployeeDialog", "Update Employee", nullptr));
        label_cin->setText(QCoreApplication::translate("UpdateEmployeeDialog", "CIN:", nullptr));
        label_lastName->setText(QCoreApplication::translate("UpdateEmployeeDialog", "Last Name:", nullptr));
        label_firstName->setText(QCoreApplication::translate("UpdateEmployeeDialog", "First Name:", nullptr));
        label_dateOfBirth->setText(QCoreApplication::translate("UpdateEmployeeDialog", "Date of Birth:", nullptr));
        dateOfBirthEdit->setDisplayFormat(QCoreApplication::translate("UpdateEmployeeDialog", "d/M/yyyy", nullptr));
        label_phone->setText(QCoreApplication::translate("UpdateEmployeeDialog", "Phone:", nullptr));
        label_email->setText(QCoreApplication::translate("UpdateEmployeeDialog", "Email:", nullptr));
        label_gender->setText(QCoreApplication::translate("UpdateEmployeeDialog", "Gender:", nullptr));
        radioButton_H->setText(QCoreApplication::translate("UpdateEmployeeDialog", "Homme", nullptr));
        radioButton_F->setText(QCoreApplication::translate("UpdateEmployeeDialog", "Femme", nullptr));
        label_salary->setText(QCoreApplication::translate("UpdateEmployeeDialog", "Salary:", nullptr));
        label_dateOfHire->setText(QCoreApplication::translate("UpdateEmployeeDialog", "Date of Hire:", nullptr));
        dateOfHireEdit->setDisplayFormat(QCoreApplication::translate("UpdateEmployeeDialog", "d/M/yyyy", nullptr));
        label_specialty->setText(QCoreApplication::translate("UpdateEmployeeDialog", "Specialty:", nullptr));
        specialtyComboBox->setItemText(0, QCoreApplication::translate("UpdateEmployeeDialog", "Informatique", nullptr));
        specialtyComboBox->setItemText(1, QCoreApplication::translate("UpdateEmployeeDialog", "Finance", nullptr));
        specialtyComboBox->setItemText(2, QCoreApplication::translate("UpdateEmployeeDialog", "Communication", nullptr));

        label_image->setText(QCoreApplication::translate("UpdateEmployeeDialog", "Image:", nullptr));
        imageLabel->setText(QCoreApplication::translate("UpdateEmployeeDialog", "(No Image)", nullptr));
        selectImageButton->setText(QCoreApplication::translate("UpdateEmployeeDialog", "Select New Image", nullptr));
        label_role->setText(QCoreApplication::translate("UpdateEmployeeDialog", "Role:", nullptr));
        roleComboBox->setItemText(0, QCoreApplication::translate("UpdateEmployeeDialog", "Admin", nullptr));
        roleComboBox->setItemText(1, QCoreApplication::translate("UpdateEmployeeDialog", "Employee", nullptr));
        roleComboBox->setItemText(2, QCoreApplication::translate("UpdateEmployeeDialog", "Manager", nullptr));

        confirmButton->setText(QCoreApplication::translate("UpdateEmployeeDialog", "Confirm", nullptr));
        cancelButton->setText(QCoreApplication::translate("UpdateEmployeeDialog", "Cancel", nullptr));
    } // retranslateUi

};

namespace Ui {
    class UpdateEmployeeDialog: public Ui_UpdateEmployeeDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_UPDATECLIENTDIALOG_H
