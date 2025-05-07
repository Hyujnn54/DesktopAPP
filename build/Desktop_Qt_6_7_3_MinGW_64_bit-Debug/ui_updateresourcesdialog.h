/********************************************************************************
** Form generated from reading UI file 'updateresourcesdialog.ui'
**
** Created by: Qt User Interface Compiler version 6.7.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_UPDATERESOURCESDIALOG_H
#define UI_UPDATERESOURCESDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAbstractButton>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDateEdit>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_UpdateResourceDialog
{
public:
    QVBoxLayout *verticalLayout;
    QFormLayout *formLayout;
    QLabel *nameLabel;
    QLineEdit *nameLineEdit;
    QLabel *typeLabel;
    QComboBox *typeComboBox;
    QLabel *brandLabel;
    QLineEdit *brandLineEdit;
    QLabel *quantityLabel;
    QSpinBox *quantitySpinBox;
    QLabel *purchaseDateLabel;
    QDateEdit *purchaseDateEdit;
    QLabel *imageLabel;
    QHBoxLayout *horizontalLayout;
    QPushButton *selectImageButton;
    QLabel *imagePathLabel;
    QLabel *imagePreviewLabel;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *UpdateResourceDialog)
    {
        if (UpdateResourceDialog->objectName().isEmpty())
            UpdateResourceDialog->setObjectName("UpdateResourceDialog");
        UpdateResourceDialog->resize(520, 430);
        verticalLayout = new QVBoxLayout(UpdateResourceDialog);
        verticalLayout->setObjectName("verticalLayout");
        formLayout = new QFormLayout();
        formLayout->setObjectName("formLayout");
        nameLabel = new QLabel(UpdateResourceDialog);
        nameLabel->setObjectName("nameLabel");

        formLayout->setWidget(0, QFormLayout::LabelRole, nameLabel);

        nameLineEdit = new QLineEdit(UpdateResourceDialog);
        nameLineEdit->setObjectName("nameLineEdit");

        formLayout->setWidget(0, QFormLayout::FieldRole, nameLineEdit);

        typeLabel = new QLabel(UpdateResourceDialog);
        typeLabel->setObjectName("typeLabel");

        formLayout->setWidget(1, QFormLayout::LabelRole, typeLabel);

        typeComboBox = new QComboBox(UpdateResourceDialog);
        typeComboBox->addItem(QString());
        typeComboBox->addItem(QString());
        typeComboBox->addItem(QString());
        typeComboBox->addItem(QString());
        typeComboBox->setObjectName("typeComboBox");

        formLayout->setWidget(1, QFormLayout::FieldRole, typeComboBox);

        brandLabel = new QLabel(UpdateResourceDialog);
        brandLabel->setObjectName("brandLabel");

        formLayout->setWidget(2, QFormLayout::LabelRole, brandLabel);

        brandLineEdit = new QLineEdit(UpdateResourceDialog);
        brandLineEdit->setObjectName("brandLineEdit");

        formLayout->setWidget(2, QFormLayout::FieldRole, brandLineEdit);

        quantityLabel = new QLabel(UpdateResourceDialog);
        quantityLabel->setObjectName("quantityLabel");

        formLayout->setWidget(3, QFormLayout::LabelRole, quantityLabel);

        quantitySpinBox = new QSpinBox(UpdateResourceDialog);
        quantitySpinBox->setObjectName("quantitySpinBox");
        quantitySpinBox->setMinimum(1);
        quantitySpinBox->setMaximum(9999);

        formLayout->setWidget(3, QFormLayout::FieldRole, quantitySpinBox);

        purchaseDateLabel = new QLabel(UpdateResourceDialog);
        purchaseDateLabel->setObjectName("purchaseDateLabel");

        formLayout->setWidget(4, QFormLayout::LabelRole, purchaseDateLabel);

        purchaseDateEdit = new QDateEdit(UpdateResourceDialog);
        purchaseDateEdit->setObjectName("purchaseDateEdit");
        purchaseDateEdit->setCalendarPopup(true);

        formLayout->setWidget(4, QFormLayout::FieldRole, purchaseDateEdit);


        verticalLayout->addLayout(formLayout);

        imageLabel = new QLabel(UpdateResourceDialog);
        imageLabel->setObjectName("imageLabel");

        verticalLayout->addWidget(imageLabel);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        selectImageButton = new QPushButton(UpdateResourceDialog);
        selectImageButton->setObjectName("selectImageButton");

        horizontalLayout->addWidget(selectImageButton);

        imagePathLabel = new QLabel(UpdateResourceDialog);
        imagePathLabel->setObjectName("imagePathLabel");

        horizontalLayout->addWidget(imagePathLabel);


        verticalLayout->addLayout(horizontalLayout);

        imagePreviewLabel = new QLabel(UpdateResourceDialog);
        imagePreviewLabel->setObjectName("imagePreviewLabel");
        imagePreviewLabel->setMinimumSize(QSize(0, 120));
        imagePreviewLabel->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(imagePreviewLabel);

        buttonBox = new QDialogButtonBox(UpdateResourceDialog);
        buttonBox->setObjectName("buttonBox");
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        verticalLayout->addWidget(buttonBox);


        retranslateUi(UpdateResourceDialog);
        QObject::connect(buttonBox, &QDialogButtonBox::accepted, UpdateResourceDialog, qOverload<>(&QDialog::accept));
        QObject::connect(buttonBox, &QDialogButtonBox::rejected, UpdateResourceDialog, qOverload<>(&QDialog::reject));

        QMetaObject::connectSlotsByName(UpdateResourceDialog);
    } // setupUi

    void retranslateUi(QDialog *UpdateResourceDialog)
    {
        UpdateResourceDialog->setWindowTitle(QCoreApplication::translate("UpdateResourceDialog", "Update Resource", nullptr));
        nameLabel->setText(QCoreApplication::translate("UpdateResourceDialog", "Name:", nullptr));
        typeLabel->setText(QCoreApplication::translate("UpdateResourceDialog", "Type:", nullptr));
        typeComboBox->setItemText(0, QCoreApplication::translate("UpdateResourceDialog", "Electronics", nullptr));
        typeComboBox->setItemText(1, QCoreApplication::translate("UpdateResourceDialog", "Cleaning Supplies", nullptr));
        typeComboBox->setItemText(2, QCoreApplication::translate("UpdateResourceDialog", "Kitchen Supplies", nullptr));
        typeComboBox->setItemText(3, QCoreApplication::translate("UpdateResourceDialog", "Stationery", nullptr));

        brandLabel->setText(QCoreApplication::translate("UpdateResourceDialog", "Brand:", nullptr));
        quantityLabel->setText(QCoreApplication::translate("UpdateResourceDialog", "Quantity:", nullptr));
        purchaseDateLabel->setText(QCoreApplication::translate("UpdateResourceDialog", "Purchase Date:", nullptr));
        imageLabel->setText(QCoreApplication::translate("UpdateResourceDialog", "Image:", nullptr));
        selectImageButton->setText(QCoreApplication::translate("UpdateResourceDialog", "Select Image", nullptr));
        imagePathLabel->setText(QCoreApplication::translate("UpdateResourceDialog", "No image selected", nullptr));
        imagePreviewLabel->setText(QCoreApplication::translate("UpdateResourceDialog", "Image preview", nullptr));
    } // retranslateUi

};

namespace Ui {
    class UpdateResourceDialog: public Ui_UpdateResourceDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_UPDATERESOURCESDIALOG_H
