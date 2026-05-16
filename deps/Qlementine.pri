QLEMENTINE_ROOT = $$PWD/qlementine/lib

SOURCES += \
    $$QLEMENTINE_ROOT/src/animation/WidgetAnimationManager.cpp \
    $$QLEMENTINE_ROOT/src/animation/WidgetAnimator.cpp \
    $$QLEMENTINE_ROOT/src/resources/ResourceInitialization.cpp \
    $$QLEMENTINE_ROOT/src/style/Delegates.cpp \
    $$QLEMENTINE_ROOT/src/style/EventFilters.cpp \
    $$QLEMENTINE_ROOT/src/style/QlementineStyle.cpp \
    $$QLEMENTINE_ROOT/src/style/Theme.cpp \
    $$QLEMENTINE_ROOT/src/style/ThemeManager.cpp \
    $$QLEMENTINE_ROOT/src/tools/ThemeEditor.cpp \
    $$QLEMENTINE_ROOT/src/utils/BadgeUtils.cpp \
    $$QLEMENTINE_ROOT/src/utils/ColorUtils.cpp \
    $$QLEMENTINE_ROOT/src/utils/FontUtils.cpp \
    $$QLEMENTINE_ROOT/src/utils/GeometryUtils.cpp \
    $$QLEMENTINE_ROOT/src/utils/IconUtils.cpp \
    $$QLEMENTINE_ROOT/src/utils/ImageUtils.cpp \
    $$QLEMENTINE_ROOT/src/utils/LayoutUtils.cpp \
    $$QLEMENTINE_ROOT/src/utils/MenuUtils.cpp \
    $$QLEMENTINE_ROOT/src/utils/PrimitiveUtils.cpp \
    $$QLEMENTINE_ROOT/src/utils/RadiusesF.cpp \
    $$QLEMENTINE_ROOT/src/utils/StateUtils.cpp \
    $$QLEMENTINE_ROOT/src/utils/StyleUtils.cpp \
    $$QLEMENTINE_ROOT/src/utils/WidgetUtils.cpp \
    $$QLEMENTINE_ROOT/src/widgets/AboutDialog.cpp \
    $$QLEMENTINE_ROOT/src/widgets/AbstractItemListWidget.cpp \
    $$QLEMENTINE_ROOT/src/widgets/Action.cpp \
    $$QLEMENTINE_ROOT/src/widgets/ActionButton.cpp \
    $$QLEMENTINE_ROOT/src/widgets/ColorButton.cpp \
    $$QLEMENTINE_ROOT/src/widgets/ColorEditor.cpp \
    $$QLEMENTINE_ROOT/src/widgets/CommandLinkButton.cpp \
    $$QLEMENTINE_ROOT/src/widgets/Expander.cpp \
    $$QLEMENTINE_ROOT/src/widgets/IconWidget.cpp \
    $$QLEMENTINE_ROOT/src/widgets/Label.cpp \
    $$QLEMENTINE_ROOT/src/widgets/LineEdit.cpp \
    $$QLEMENTINE_ROOT/src/widgets/LoadingSpinner.cpp \
    $$QLEMENTINE_ROOT/src/widgets/Menu.cpp \
    $$QLEMENTINE_ROOT/src/widgets/NavigationBar.cpp \
    $$QLEMENTINE_ROOT/src/widgets/NotificationBadge.cpp \
    $$QLEMENTINE_ROOT/src/widgets/PlainTextEdit.cpp \
    $$QLEMENTINE_ROOT/src/widgets/Popover.cpp \
    $$QLEMENTINE_ROOT/src/widgets/PopoverButton.cpp \
    $$QLEMENTINE_ROOT/src/widgets/RoundedFocusFrame.cpp \
    $$QLEMENTINE_ROOT/src/widgets/SegmentedControl.cpp \
    $$QLEMENTINE_ROOT/src/widgets/StatusBadgeWidget.cpp \
    $$QLEMENTINE_ROOT/src/widgets/Switch.cpp

HEADERS += \
    $$QLEMENTINE_ROOT/include/oclero/qlementine/animation/WidgetAnimation.hpp \
    $$QLEMENTINE_ROOT/include/oclero/qlementine/animation/WidgetAnimationManager.hpp \
    $$QLEMENTINE_ROOT/include/oclero/qlementine/animation/WidgetAnimator.hpp \
    $$QLEMENTINE_ROOT/include/oclero/qlementine/Common.hpp \
    $$QLEMENTINE_ROOT/include/oclero/qlementine/resources/ResourceInitialization.hpp \
    $$QLEMENTINE_ROOT/include/oclero/qlementine/style/Delegates.hpp \
    $$QLEMENTINE_ROOT/include/oclero/qlementine/style/QlementineStyle.hpp \
    $$QLEMENTINE_ROOT/include/oclero/qlementine/style/QlementineStyleOption.hpp \
    $$QLEMENTINE_ROOT/include/oclero/qlementine/style/Theme.hpp \
    $$QLEMENTINE_ROOT/include/oclero/qlementine/style/ThemeManager.hpp \
    $$QLEMENTINE_ROOT/include/oclero/qlementine/tools/ThemeEditor.hpp \
    $$QLEMENTINE_ROOT/include/oclero/qlementine/utils/BadgeUtils.hpp \
    $$QLEMENTINE_ROOT/include/oclero/qlementine/utils/BlurUtils.hpp \
    $$QLEMENTINE_ROOT/include/oclero/qlementine/utils/ColorUtils.hpp \
    $$QLEMENTINE_ROOT/include/oclero/qlementine/utils/FontUtils.hpp \
    $$QLEMENTINE_ROOT/include/oclero/qlementine/utils/GeometryUtils.hpp \
    $$QLEMENTINE_ROOT/include/oclero/qlementine/utils/IconUtils.hpp \
    $$QLEMENTINE_ROOT/include/oclero/qlementine/utils/ImageUtils.hpp \
    $$QLEMENTINE_ROOT/include/oclero/qlementine/utils/LayoutUtils.hpp \
    $$QLEMENTINE_ROOT/include/oclero/qlementine/utils/MenuUtils.hpp \
    $$QLEMENTINE_ROOT/include/oclero/qlementine/utils/PrimitiveUtils.hpp \
    $$QLEMENTINE_ROOT/include/oclero/qlementine/utils/RadiusesF.hpp \
    $$QLEMENTINE_ROOT/include/oclero/qlementine/utils/StateUtils.hpp \
    $$QLEMENTINE_ROOT/include/oclero/qlementine/utils/StyleUtils.hpp \
    $$QLEMENTINE_ROOT/include/oclero/qlementine/utils/WidgetUtils.hpp \
    $$QLEMENTINE_ROOT/include/oclero/qlementine/widgets/AboutDialog.hpp \
    $$QLEMENTINE_ROOT/include/oclero/qlementine/widgets/AbstractItemListWidget.hpp \
    $$QLEMENTINE_ROOT/include/oclero/qlementine/widgets/Action.hpp \
    $$QLEMENTINE_ROOT/include/oclero/qlementine/widgets/ActionButton.hpp \
    $$QLEMENTINE_ROOT/include/oclero/qlementine/widgets/ColorButton.hpp \
    $$QLEMENTINE_ROOT/include/oclero/qlementine/widgets/ColorEditor.hpp \
    $$QLEMENTINE_ROOT/include/oclero/qlementine/widgets/CommandLinkButton.hpp \
    $$QLEMENTINE_ROOT/include/oclero/qlementine/widgets/Expander.hpp \
    $$QLEMENTINE_ROOT/include/oclero/qlementine/widgets/IconWidget.hpp \
    $$QLEMENTINE_ROOT/include/oclero/qlementine/widgets/Label.hpp \
    $$QLEMENTINE_ROOT/include/oclero/qlementine/widgets/LineEdit.hpp \
    $$QLEMENTINE_ROOT/include/oclero/qlementine/widgets/LoadingSpinner.hpp \
    $$QLEMENTINE_ROOT/include/oclero/qlementine/widgets/Menu.hpp \
    $$QLEMENTINE_ROOT/include/oclero/qlementine/widgets/NavigationBar.hpp \
    $$QLEMENTINE_ROOT/include/oclero/qlementine/widgets/NotificationBadge.hpp \
    $$QLEMENTINE_ROOT/include/oclero/qlementine/widgets/PlainTextEdit.hpp \
    $$QLEMENTINE_ROOT/include/oclero/qlementine/widgets/Popover.hpp \
    $$QLEMENTINE_ROOT/include/oclero/qlementine/widgets/PopoverButton.hpp \
    $$QLEMENTINE_ROOT/include/oclero/qlementine/widgets/RoundedFocusFrame.hpp \
    $$QLEMENTINE_ROOT/include/oclero/qlementine/widgets/SegmentedControl.hpp \
    $$QLEMENTINE_ROOT/include/oclero/qlementine/widgets/StatusBadgeWidget.hpp \
    $$QLEMENTINE_ROOT/include/oclero/qlementine/widgets/Switch.hpp \
    $$QLEMENTINE_ROOT/src/style/EventFilters.hpp \
    $$QLEMENTINE_ROOT/src/style/eventFilters/ComboboxItemViewFilter.hpp \
    $$QLEMENTINE_ROOT/src/style/eventFilters/CommandLinkButtonEventFilter.hpp \
    $$QLEMENTINE_ROOT/src/style/eventFilters/LineEditButtonEventFilter.hpp \
    $$QLEMENTINE_ROOT/src/style/eventFilters/LineEditMenuEventFilter.hpp \
    $$QLEMENTINE_ROOT/src/style/eventFilters/MenuEventFilter.hpp \
    $$QLEMENTINE_ROOT/src/style/eventFilters/MouseWheelBlockerEventFilter.hpp \
    $$QLEMENTINE_ROOT/src/style/eventFilters/TabBarEventFilter.hpp \
    $$QLEMENTINE_ROOT/src/style/eventFilters/TextEditEventFilter.hpp \
    $$QLEMENTINE_ROOT/src/style/eventFilters/WidgetWithFocusFrameEventFilter.hpp

RESOURCES += \
    $$QLEMENTINE_ROOT/resources/qlementine.qrc \
    $$QLEMENTINE_ROOT/resources/qlementine_font_inter.qrc \
    $$QLEMENTINE_ROOT/resources/qlementine_font_roboto.qrc

INCLUDEPATH += \
    $$QLEMENTINE_ROOT/include \
    $$QLEMENTINE_ROOT/src

QT += core widgets svg
QMAKE_CXXFLAGS += -Wall -Wextra -Werror
