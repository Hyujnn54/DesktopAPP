#include "calendarhoveritem.h"
#include <QHoverEvent>
#include <QTableView>
#include <QToolTip>

CalendarHoverItem::CalendarHoverItem(QCalendarWidget* cal, const QMap<QDate, int>* counts, QObject* parent)
    : QObject(parent), calendar(cal), consultationCounts(counts) {}

bool CalendarHoverItem::eventFilter(QObject* watched, QEvent* event) {
    if (watched == calendar && event->type() == QEvent::HoverMove) {
        QHoverEvent* hoverEvent = static_cast<QHoverEvent*>(event);
        QPoint pos = hoverEvent->position().toPoint();

        QTableView* tableView = calendar->findChild<QTableView*>();
        QDate hoverDate;

        if (tableView) {
            QModelIndex index = tableView->indexAt(pos);
            if (index.isValid()) {
                int row = index.row();
                int col = index.column();

                QDate firstDayOfMonth = calendar->selectedDate().addDays(-calendar->selectedDate().day() + 1);
                int offset = firstDayOfMonth.dayOfWeek() - 1; // Adjust for first day of week

                int day = (row * 7) + col - offset + 1;
                if (day > 0 && day <= firstDayOfMonth.daysInMonth()) {
                    hoverDate = QDate(firstDayOfMonth.year(), firstDayOfMonth.month(), day);
                } else {
                    hoverDate = calendar->selectedDate();
                }
            }
        } else {
            hoverDate = calendar->selectedDate();
        }

        if (hoverDate.isValid()) {
            int count = consultationCounts->value(hoverDate, 0);
            if (count > 0) {
                QString tooltipText = QString("%1: %2 consultation(s)")
                .arg(hoverDate.toString("yyyy-MM-dd"))
                    .arg(count);
                QToolTip::showText(calendar->mapToGlobal(hoverEvent->position().toPoint()), tooltipText, calendar);
            } else {
                QToolTip::hideText();
            }
        }
    }
    return QObject::eventFilter(watched, event);
}
