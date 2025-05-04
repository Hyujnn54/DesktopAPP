#include "updateresourcesdialog.h"
#include "ui_updateresourcesdialog.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QPixmap>
#include <QBuffer>
#include <QDebug>

UpdateResourceDialog::UpdateResourceDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UpdateResourceDialog),
    imageData(QByteArray())
{
    ui->setupUi(this);
    
    // Set the current date as default
    ui->purchaseDateEdit->setDate(QDate::currentDate());
    
    // Connect signals and slots
    connect(ui->buttonBox, &QDialogButtonBox::accepted, this, &UpdateResourceDialog::onAccepted);
}

UpdateResourceDialog::~UpdateResourceDialog()
{
    delete ui;
}

void UpdateResourceDialog::setResourceData([[maybe_unused]] int resourceId, const QString &name, const QString &type,
                                          const QString &brand, int quantity, const QDate &purchaseDate, 
                                          const QByteArray &imageData)
{
    // Removed resourceIdLineEdit since it's no longer in the UI
    ui->nameLineEdit->setText(name);
    
    // Set the type in the combobox
    int index = ui->typeComboBox->findText(type);
    if (index != -1) {
        ui->typeComboBox->setCurrentIndex(index);
    }
    
    ui->brandLineEdit->setText(brand);
    ui->quantitySpinBox->setValue(quantity);
    ui->purchaseDateEdit->setDate(purchaseDate);
    
    // Store the image data and update preview
    this->imageData = imageData;
    if (!imageData.isEmpty()) {
        QPixmap pixmap;
        if (pixmap.loadFromData(imageData)) {
            ui->imagePreviewLabel->setPixmap(pixmap.scaled(200, 120, Qt::KeepAspectRatio, Qt::SmoothTransformation));
            ui->imagePathLabel->setText("Image loaded");
        } else {
            ui->imagePreviewLabel->setText("Failed to load image");
        }
    }
}

QString UpdateResourceDialog::getName() const
{
    return ui->nameLineEdit->text().trimmed();
}

QString UpdateResourceDialog::getType() const
{
    return ui->typeComboBox->currentText();
}

QString UpdateResourceDialog::getBrand() const
{
    return ui->brandLineEdit->text().trimmed();
}

int UpdateResourceDialog::getQuantity() const
{
    return ui->quantitySpinBox->value();
}

QDate UpdateResourceDialog::getPurchaseDate() const
{
    return ui->purchaseDateEdit->date();
}

QByteArray UpdateResourceDialog::getImageData() const
{
    return imageData;
}

void UpdateResourceDialog::on_selectImageButton_clicked()
{
    QString filePath = QFileDialog::getOpenFileName(this, "Select Image", "", "Image Files (*.png *.jpg *.jpeg *.bmp)");
    
    if (!filePath.isEmpty()) {
        QFile file(filePath);
        if (file.open(QIODevice::ReadOnly)) {
            imageData = file.readAll();
            file.close();
            
            QPixmap pixmap;
            if (pixmap.loadFromData(imageData)) {
                ui->imagePreviewLabel->setPixmap(pixmap.scaled(200, 120, Qt::KeepAspectRatio, Qt::SmoothTransformation));
                ui->imagePathLabel->setText(filePath);
            } else {
                ui->imagePreviewLabel->setText("Failed to load image");
                imageData.clear();
            }
        } else {
            QMessageBox::warning(this, "Error", "Failed to open the selected file.");
        }
    }
}

void UpdateResourceDialog::onAccepted()
{
    if (validate()) {
        accept();
    }
}

bool UpdateResourceDialog::validate()
{
    // Name must not be empty and must start with uppercase
    QString name = getName();
    if (name.isEmpty()) {
        QMessageBox::warning(this, "Validation Error", "Name cannot be empty.");
        return false;
    }
    
    if (!name.isEmpty() && !name[0].isUpper()) {
        QMessageBox::warning(this, "Validation Error", "Name must start with an uppercase letter.");
        return false;
    }
    
    // Brand must not be empty and must start with uppercase
    QString brand = getBrand();
    if (brand.isEmpty()) {
        QMessageBox::warning(this, "Validation Error", "Brand cannot be empty.");
        return false;
    }
    
    if (!brand.isEmpty() && !brand[0].isUpper()) {
        QMessageBox::warning(this, "Validation Error", "Brand must start with an uppercase letter.");
        return false;
    }
    
    // Quantity must be positive
    if (getQuantity() <= 0) {
        QMessageBox::warning(this, "Validation Error", "Quantity must be greater than zero.");
        return false;
    }
    
    // Purchase date must not be in the future
    if (getPurchaseDate() > QDate::currentDate()) {
        QMessageBox::warning(this, "Validation Error", "Purchase date cannot be in the future.");
        return false;
    }
    
    // Image must be provided
    if (imageData.isEmpty()) {
        QMessageBox::warning(this, "Validation Error", "Please select an image.");
        return false;
    }
    
    return true;
}
