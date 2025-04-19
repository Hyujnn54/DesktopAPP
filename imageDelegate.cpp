#include "imageDelegate.h"
#include <QPainter>
#include <QSqlQueryModel>
#include <QImage>
#include <QDebug>

ImageDelegate::ImageDelegate(QObject *parent)
    : QStyledItemDelegate(parent)
{
}

void ImageDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if (index.column() == 11) { // Assuming IMAGE is the 11th column (0-based index)
        QByteArray imageData = index.model()->data(index, Qt::DisplayRole).toByteArray();
        if (!imageData.isEmpty()) {
            QImage image;
            if (image.loadFromData(imageData)) {
                // Use the cell's rectangle for dynamic scaling
                QRect targetRect = option.rect.adjusted(2, 2, -2, -2); // Add small padding
                QPixmap pixmap = QPixmap::fromImage(image);

                // Scale the pixmap to fit the cell while preserving aspect ratio and high quality
                QPixmap scaledPixmap = pixmap.scaled(targetRect.size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);

                // Center the scaled pixmap in the cell
                int x = targetRect.x() + (targetRect.width() - scaledPixmap.width()) / 2;
                int y = targetRect.y() + (targetRect.height() - scaledPixmap.height()) / 2;
                painter->drawPixmap(QPoint(x, y), scaledPixmap);
            } else {
                painter->drawText(option.rect, Qt::AlignCenter, "Invalid Image");
                qDebug() << "Failed to load image from BLOB for index:" << index.row();
            }
        } else {
            painter->drawText(option.rect, Qt::AlignCenter, "No Image");
            qDebug() << "No image data for index:" << index.row();
        }
    } else {
        QStyledItemDelegate::paint(painter, option, index); // Default rendering for other columns
    }
}

QSize ImageDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if (index.column() == 11) {
        // Return a reasonable default size, but let the cell size dictate the final render
        return QSize(120, 120); // Slightly larger default to allow clearer images
    }
    return QStyledItemDelegate::sizeHint(option, index);
}
