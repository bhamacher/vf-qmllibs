import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.12
import QmlHelpers 1.0
import ZeraLocale 1.0

Item {
    // TODO: introduce description/unit as in ZLineEdit?
    Layout.alignment: Qt.AlignVCenter
    Layout.minimumWidth: sBox.width
    height: parent.height

    // public interface
    property alias stepSize: sBox.stepSize
    property alias spinBox: sBox
    property bool readOnly: false

    property var validator
    property string text: "" // locale C
    readonly property bool acceptableInput: hasValidInput()
    property bool changeOnFocusLost: true

    // overridables
    function doApplyInput(newText) {return true} //  (return true: apply immediate)
    function hasAlteredValue() {
        var decimals = isDouble ? validator.decimals : 0
        return TextHelper.hasAlteredValue(isNumeric, isDouble, decimals, tField.text, text)
    }
    function hasValidInput() {
        return tField.acceptableInput && TextHelper.hasValidInput(isDouble, tField.text)
    }
    function discardInput() {
        tField.text = ZLocale.strToLocal(text, isNumeric, isDouble)
    }
    function activeFocusChange(actFocus) {
        baseActiveFocusChange(actFocus)
    }

    // signal handler
    onTextChanged: {
        if(!inApply) {
            sBox.value = sBox.valueFromText(text, ZLocale.locale)
            discardInput()
        }
        inApply = false
    }
    onValidatorChanged: {
        sBox.validator = validator
        if(isNumeric) {
            if(isDouble) {
                sBox.from = validator.bottom*Math.pow(10, validator.decimals)
                sBox.to = validator.top*Math.pow(10, validator.decimals)
            }
            else {
                sBox.from = validator.bottom
                sBox.to = validator.top
            }
            sBox.inputMethodHints = Qt.ImhFormattedNumbersOnly
        }
        else {
            sBox.inputMethodHints = Qt.ImhNoAutoUppercase
        }
    }
    onReadOnlyChanged: {
        sBox.editable = !readOnly
    }
    onLocaleNameChanged: {
        sBox.locale = ZLocale.locale
        discardInput()
    }

    // base implementations
    function baseActiveFocusChange(actFocus) {
        if(changeOnFocusLost && !inFocusKill && !actFocus) {
            if(hasAlteredValue()) {
                if(hasValidInput()) {
                    applyInput()
                }
                else {
                    discardInput()
                }
            }
        }
        // Hmm - maybe we should add an option for this...
        /*else {
            selectAll()
        }*/
    }

    property var tField: sBox.contentItem

    // bit of a hack to check for IntValidator / DoubleValidator to detect a numeric field
    readonly property bool isNumeric: validator !== undefined && 'bottom' in sBox.validator && 'top' in sBox.validator
    readonly property bool isDouble: isNumeric && 'decimals' in sBox.validator
    property bool inApply: false
    property bool inFocusKill: false
    readonly property string localeName: ZLocale.localeName
    function applyInput() {
        if(hasValidInput())
        {
            if(hasAlteredValue())
            {
                inApply = true
                var newText = ZLocale.strToCLocale(tField.text, isNumeric, isDouble)
                if(doApplyInput(newText)) {
                    text = newText
                    inApply = false
                }
            }
            // we changed text but did not change value
            else {
                discardInput()
            }
        }
        else {
            discardInput()
        }
    }

    // controls
    SpinBox {
        id: sBox
        height: parent.height
        bottomPadding: 8
        editable: true
        inputMethodHints: Qt.ImhFormattedNumbersOnly

        // overrides
        textFromValue: function(value, locale) {
            if (isNumeric) {
                if(isDouble) {
                    var val = value / Math.pow(10, validator.decimals)
                    return ZLocale.strToLocal(val.toString(), isNumeric, isDouble)
                }
                else {
                    return ZLocale.strToLocal(value.toString(), isNumeric, isDouble)
                }
            }
            else {
                // TODO
                return ""
            }
        }
        valueFromText: function(text, locale) {
            if (isNumeric) {
                if(isDouble) {
                    var str = ZLocale.strToLocal(text, isNumeric, isDouble)
                    return Number.fromLocaleString(locale, str)*Math.pow(10, validator.decimals)
                }
                else {
                    return parseInt(text, 10)
                }
            }
            else {
                // TODO
                return 0
            }

        }

        // Events
        Keys.onReturnPressed: {
            // Hmm try to get same behaviour as ZLineEdit
            if(hasValidInput())
            {
                applyInput()
                inFocusKill = true
                focus = false
                inFocusKill = false
            }
        }
        Keys.onEscapePressed: {
            discardInput()
            inFocusKill = true
            focus = false
            inFocusKill = false
        }
        /* Avoid QML magic: when the cursor is at start/end position,
        left/right keys are used to change tab. We don't want that */
        Keys.onLeftPressed: {
            if(tField.cursorPosition > 0 || tField.selectedText !== "") {
                event.accepted = false;
            }
        }
        Keys.onRightPressed: {
            if(tField.cursorPosition < displayText.length || tField.selectedText !== "") {
                event.accepted = false;
            }
        }
        onValueModified: {
            if(!inApply) {
                // TODO Text spins
                tField.text = textFromValue(value, ZLocale.locale)
                if(!sBox.focus)
                    applyInput()
            }
        }
        onActiveFocusChanged: {
            activeFocusChange(activeFocus)
        }

        // Background rects
        Rectangle {
            anchors.fill: tField
            anchors.bottomMargin: -4
            color: "red"
            opacity: 0.2
            visible: hasValidInput() === false && !readOnly
        }
        Rectangle {
            anchors.fill: tField
            anchors.bottomMargin: -4
            color: "green"
            opacity: 0.2
            visible: hasValidInput() && !readOnly && hasAlteredValue()
        }

    }
}
