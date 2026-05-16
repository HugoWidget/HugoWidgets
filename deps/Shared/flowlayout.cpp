#include "flowlayout.h"

FlowLayout::FlowLayout(QWidget *parent, int margin, int hSpacing, int vSpacing)
    : QLayout(parent), m_hSpace(hSpacing), m_vSpace(vSpacing) {
    setContentsMargins(margin, margin, margin, margin);
}

FlowLayout::~FlowLayout() {
    QLayoutItem *item;
    while ((item = takeAt(0)))
        delete item;
}

void FlowLayout::addItem(QLayoutItem *item) { m_items.append(item); }

int FlowLayout::horizontalSpacing() const {
    if (m_hSpace >= 0)
        return m_hSpace;
    else
        return smartSpacing(QStyle::PM_LayoutHorizontalSpacing);
}

int FlowLayout::verticalSpacing() const {
    if (m_vSpace >= 0)
        return m_vSpace;
    else
        return smartSpacing(QStyle::PM_LayoutVerticalSpacing);
}

Qt::Orientations FlowLayout::expandingDirections() const {
    return Qt::Orientation();
}

bool FlowLayout::hasHeightForWidth() const { return true; }
int FlowLayout::heightForWidth(int width) const {
    return doLayout(QRect(0, 0, width, 0), true);
}

int FlowLayout::count() const { return m_items.size(); }
QLayoutItem *FlowLayout::itemAt(int index) const {
    return m_items.value(index);
}
QLayoutItem *FlowLayout::takeAt(int index) {
    if (index >= 0 && index < m_items.size())
        return m_items.takeAt(index);
    return nullptr;
}

void FlowLayout::setGeometry(const QRect &rect) {
    QLayout::setGeometry(rect);
    doLayout(rect, false);
}

QSize FlowLayout::sizeHint() const { return minimumSize(); }
QSize FlowLayout::minimumSize() const {
    QSize size;
    for (const QLayoutItem *item : m_items)
        size = size.expandedTo(item->minimumSize());
    size += QSize(2 * contentsMargins().left(), 2 * contentsMargins().top());
    return size;
}

int FlowLayout::doLayout(const QRect &rect, bool testOnly) const {
    int left, top, right, bottom;
    getContentsMargins(&left, &top, &right, &bottom);
    QRect effectiveRect = rect.adjusted(+left, +top, -right, -bottom);
    int x = effectiveRect.x();
    int y = effectiveRect.y();
    int lineHeight = 0;
    int index = 0;
    int itemCount = m_items.size();

    while (index < itemCount) {
        QList<QLayoutItem *> lineItems;
        QList<int> minWidths, maxWidths;
        int totalMinWidth = 0;
        int totalMaxWidth = 0;
        int lineStartIndex = index;

        while (index < itemCount) {
            QLayoutItem *item = m_items[index];
            QWidget *w = nullptr;
            if (QWidgetItem *widgetItem = dynamic_cast<QWidgetItem *>(item))
                w = widgetItem->widget();

            int minW = w ? w->minimumWidth() : item->sizeHint().width();
            int maxW = w ? w->maximumWidth() : item->sizeHint().width();
            if (maxW == QWIDGETSIZE_MAX)
                maxW = effectiveRect.width();

            int spacing = lineItems.isEmpty() ? 0 : horizontalSpacing();
            int newTotalMinWidth = totalMinWidth + minW + spacing;
            int newTotalMaxWidth = totalMaxWidth + maxW + spacing;

            if (newTotalMinWidth > effectiveRect.width())
                break;

            int available = effectiveRect.width() - newTotalMinWidth;
            int extraCapacity = (totalMaxWidth + maxW) -
                                (totalMinWidth + minW);

            if (available > extraCapacity) {
                lineItems.append(item);
                minWidths.append(minW);
                maxWidths.append(maxW);
                totalMinWidth = newTotalMinWidth;
                totalMaxWidth = newTotalMaxWidth;
                index++;
                continue;
            } else {
                lineItems.append(item);
                minWidths.append(minW);
                maxWidths.append(maxW);
                totalMinWidth = newTotalMinWidth;
                totalMaxWidth = newTotalMaxWidth;
                index++;
                break;
            }
        }

        if (lineItems.isEmpty()) {
            index++;
            continue;
        }

        int availableWidth =
            effectiveRect.width() - totalMinWidth;
        QList<int> actualWidths = minWidths;
        int remaining = availableWidth;
        while (remaining > 0) {
            bool anyIncreased = false;
            for (int i = 0; i < lineItems.size(); ++i) {
                if (actualWidths[i] < maxWidths[i]) {
                    actualWidths[i]++;
                    remaining--;
                    anyIncreased = true;
                    if (remaining == 0)
                        break;
                }
            }
            if (!anyIncreased)
                break;
        }

        int currentX = effectiveRect.x();
        int maxHeight = 0;
        for (int i = 0; i < lineItems.size(); ++i) {
            QLayoutItem *item = lineItems[i];
            int itemWidth = actualWidths[i];
            int itemHeight = item->sizeHint().height();
            if (QWidgetItem *widgetItem = dynamic_cast<QWidgetItem *>(item)) {
                QWidget *w = widgetItem->widget();
                if (w) {
                    int minH = w->minimumHeight();
                    int maxH = w->maximumHeight();
                    if (maxH >= 0 && itemHeight > maxH)
                        itemHeight = maxH;
                    if (itemHeight < minH)
                        itemHeight = minH;
                }
            }
            if (!testOnly) {
                QRect itemRect(currentX, y, itemWidth, itemHeight);
                item->setGeometry(itemRect);
            }
            currentX += itemWidth + horizontalSpacing();
            maxHeight = qMax(maxHeight, itemHeight);
        }
        y += maxHeight + verticalSpacing();
        lineHeight = maxHeight;
    }
    return y - rect.y() + bottom;
}

int FlowLayout::smartSpacing(QStyle::PixelMetric pm) const {
    QObject *parent = this->parent();
    if (!parent)
        return -1;
    if (parent->isWidgetType()) {
        QWidget *pw = static_cast<QWidget *>(parent);
        return pw->style()->pixelMetric(pm, nullptr, pw);
    }
    return static_cast<QLayout *>(parent)->spacing();
}
