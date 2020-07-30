pragma Singleton
import QtQuick 2.0
import ModuleIntrospection 1.0
import ZeraSettings 1.0
import ZeraTranslationbackend 1.0
import VeinEntity 1.0

Item {
  /**
    * @b default configuration values and utility functions
    * @todo reimplement as QObject with Q_PROPERTY / Q_INVOKABLE to get QtCreator to code complete stuff...
    */

  ZeraGlobalSettings {
    id: settings
  }

  property bool tmpStatusNewErrors: false; //replacement for static variable will not be saved in settings.json

  property int pageViewLastSelectedIndex: 0;

  readonly property int decimalPlaces: parseInt(settings.globalSettings.getOption("digits", "3"))
  function setDecimalPlaces(digits) {
    settings.globalSettings.setOption("digits", digits);
  }

  readonly property bool showAuxPhases: parseInt(settings.globalSettings.getOption("show_aux_phases", "0"))
  function setShowAuxPhases(showAux) {
    var setValue = showAux ? 1 : 0
    settings.globalSettings.setOption("show_aux_phases", setValue);
  }
  // we have to decouple change of showAuxPhases with a timer because of some
  // CPU intensive views: when the operator changes selection, it takes ages until
  // the checkbox in settings view is updated due to heavy load caused.
  property bool showAuxPhasesDecoupled: showAuxPhases
  onShowAuxPhasesChanged: {
    decoupleTimer.start()
  }
  Timer {
    id: decoupleTimer
    interval: 500
    // We must break binding on startup
    Component.onCompleted: {
      showAuxPhasesDecoupled = showAuxPhases
    }
    onTriggered: {
      stop()
      showAuxPhasesDecoupled = showAuxPhases
    }
  }

  readonly property int showFftTableAsRelative: parseInt(settings.globalSettings.getOption("fft_table_as_relative", "0"))
  function setShowFftTableAsRelative(isRelative) {
    var setValue = isRelative ? 1 : 0
    settings.globalSettings.setOption("fft_table_as_relative", setValue);
  }

  readonly property int showFftTablePhase: parseInt(settings.globalSettings.getOption("fft_table_show_phase", "0"))
  function setShowFftTablePhase(showPhase) {
    var setValue = showPhase ? 1 : 0
    settings.globalSettings.setOption("fft_table_show_phase", setValue);
  }

  readonly property int energyScaleSelection: parseInt(settings.globalSettings.getOption("energy_scale_selection", "1")) // 1 -> kWh
  function setEnergyScaleSelection(selection) {
    settings.globalSettings.setOption("energy_scale_selection", selection);
  }

  readonly property var rangePeakVisualisationEnum: {
    "RPV_ABSOLUTE" : 0,
    "RPV_ABSOLUTE_LOGSCALE" : 1,
    "RPV_RELATIVE_TO_LIMIT" : 2
  }

  readonly property var layoutStackEnum: {
    "layoutPageIndex": 0,
    "layoutRangeIndex": 1,
    "layoutLoggerIndex": 2,
    "layoutSettingsIndex": 3,
    "layoutStatusIndex": 4
  }


  readonly property real rangePeakVisualisation: parseInt(settings.globalSettings.getOption("range_peak_logarithmic", "2")) ///@todo rename config key?
  function setRangePeakVisualisation(rangePeakVisualisation) {
    if(typeof rangePeakVisualisation === "number"
        && rangePeakVisualisation >=0
        && rangePeakVisualisation < Object.keys(rangePeakVisualisationEnum).length)
    {
      settings.globalSettings.setOption("range_peak_logarithmic", rangePeakVisualisation);
    }
    else if(rangePeakVisualisationEnum[rangePeakVisualisation] !== undefined)
    {
      settings.globalSettings.setOption("range_peak_logarithmic", rangePeakVisualisationEnum[rangePeakVisualisation]);
    }
  }

  readonly property bool pagesGridViewDisplay: parseInt(settings.globalSettings.getOption("pages_grid_view", "1"))
  function setPagesGridViewDisplay(isGridView) {
    settings.globalSettings.setOption("pages_grid_view", isGridView ? 1 : 0);
  }

  readonly property int screenResolution: parseInt(settings.globalSettings.getOption("screen_resolution", "0"))
  function setScreenResolution(resolution) {
    settings.globalSettings.setOption("screen_resolution", resolution);
  }

  readonly property bool showVirtualKeyboard: parseInt(settings.globalSettings.getOption("show_virtual_keyboard", "1"))
  function setShowVirtualKeyboard(show) {
    settings.globalSettings.setOption("show_virtual_keyboard", show ? 1 : 0);
  }


  /////////////////////////////////////////////////////////////////////////////
  // Common standard margins

  readonly property real standardMargin: 0
  readonly property real standardMarginMin: 1
  readonly property real standardMarginWithMin: standardMargin > standardMarginMin ? standardMargin : standardMarginMin
  readonly property real standardTextHorizMargin: 8
  readonly property real standardTextBottomMargin: 8
  readonly property real standardComboContentScale: 1.2
  property real vkeyboardHeight: 0

  /////////////////////////////////////////////////////////////////////////////
  // Color settings...

  function getJsonColorNameByIndex(index) {
    var retVal
    var availableSystems = ["colorUL1",     // 1
                            "colorUL2",     // 2
                            "colorUL3",     // 3
                            "colorIL1",     // 4
                            "colorIL2",     // 5
                            "colorIL3",     // 6
                            "colorUAux1",   // 7
                            "colorIAux1"];  // 8
    retVal = availableSystems[index-1]
    return retVal
  }

  function getDefaultColorByIndex(index) {
    var retVal
    switch(index) {
    case 1:
      retVal = "#EEff0000";
      break;
    case 2:
      retVal = "#EEffff00";
      break;
    case 3:
      retVal = "#EE0092ff";
      break;
    case 4:
      retVal = "#EEff7755";
      break;
    case 5:
      retVal = "#EEffffbb";
      break;
    case 6:
      retVal = "#EE58acfa";
      break;
    case 7:
      retVal = "#EEffffff";
      break;
    case 8:
      retVal = "#EEcccccc";
      break;
    }
    return retVal
  }

  function systemColorByIndex(index) {
    return settings.globalSettings.getOption(getJsonColorNameByIndex(index), getDefaultColorByIndex(index))
  }

  function setSystemColorByIndex(index, color) {
    settings.globalSettings.setOption(getJsonColorNameByIndex(index), color);
  }

  function setSystemDefaultColors() {
    var index
    for (index=1; index<=8; ++index) {
      setSystemColorByIndex(index, getDefaultColorByIndex(index))
    }
  }

  readonly property color colorUL1: settings.globalSettings.getOption(getJsonColorNameByIndex(1), getDefaultColorByIndex(1))
  readonly property color colorUL2: settings.globalSettings.getOption(getJsonColorNameByIndex(2), getDefaultColorByIndex(2))
  readonly property color colorUL3: settings.globalSettings.getOption(getJsonColorNameByIndex(3), getDefaultColorByIndex(3))
  readonly property color colorIL1: settings.globalSettings.getOption(getJsonColorNameByIndex(4), getDefaultColorByIndex(4))
  readonly property color colorIL2: settings.globalSettings.getOption(getJsonColorNameByIndex(5), getDefaultColorByIndex(5))
  readonly property color colorIL3: settings.globalSettings.getOption(getJsonColorNameByIndex(6), getDefaultColorByIndex(6))
  readonly property color colorUAux1: settings.globalSettings.getOption(getJsonColorNameByIndex(7), getDefaultColorByIndex(7))
  readonly property color colorIAux1: settings.globalSettings.getOption(getJsonColorNameByIndex(8), getDefaultColorByIndex(8))

  readonly property color groupColorVoltage: settings.globalSettings.getOption("groupColor1", "lightskyblue")
  readonly property color groupColorCurrent: settings.globalSettings.getOption("groupColor2", "lawngreen")
  readonly property color groupColorReference: settings.globalSettings.getOption("groupColor3", "darkorange")

  readonly property color tableShadeColor: "#003040"

  function getColorByIndex(rangIndex, grouping) {
    var retVal;
    if(grouping)
    {
      var channelName = ModuleIntrospection.rangeIntrospection.ComponentInfo["PAR_Channel"+rangIndex+"Range"].ChannelName;
      var group1 = ModuleIntrospection.rangeIntrospection.ModuleInfo.ChannelGroup1;
      var group2 = ModuleIntrospection.rangeIntrospection.ModuleInfo.ChannelGroup2;
      var group3 = ModuleIntrospection.rangeIntrospection.ModuleInfo.ChannelGroup3;

      if(group1 !== undefined && group1.indexOf(channelName)>-1)
      {
        retVal = groupColorVoltage
      }
      else if(group2 !== undefined && group2.indexOf(channelName)>-1)
      {
        retVal = groupColorCurrent
      }
      else if(group3 !== undefined && group3.indexOf(channelName)>-1)
      {
        retVal = groupColorReference
      }
      else //index is not in group
      {
        retVal = systemColorByIndex(rangIndex)
      }
    }
    else
    {
      retVal = systemColorByIndex(rangIndex)
    }
    return retVal;
  }

  /////////////////////////////////////////////////////////////////////////////
  // Error margins

  readonly property real errorMarginUpperValue: parseFloat(settings.globalSettings.getOption("errorMarginUpperValue", "10.0"))
  function setErrorMarginUpperValue(upperLimit) {
    settings.globalSettings.setOption("errorMarginUpperValue", upperLimit);
  }
  readonly property real errorMarginLowerValue: parseFloat(settings.globalSettings.getOption("errorMarginLowerValue", "-10.0"))
  function setErrorMarginLowerValue(lowerLimit) {
    settings.globalSettings.setOption("errorMarginLowerValue", lowerLimit);
  }

  /////////////////////////////////////////////////////////////////////////////
  // Auto scale (+ helper functions)

  /* Settings/autoScaleLimit: A float number to set at which limit the value
     changes and unit is prefixed e.g for autoScaleLimit=1.2 value >= 1200 is
     changed to 1.2k */

  readonly property real autoScaleLimit: parseFloat(settings.globalSettings.getOption("auto_scale_limit", "1.0"))

  function setAutoScaleLimit(limit) {
    if(typeof limit === "string") {
      settings.globalSettings.setOption("auto_scale_limit", limit);
    }
    else  {
      settings.globalSettings.setOption("auto_scale_limit", formatNumber(limit, 3));
    }
  }
  // Auto scale float value (num) / unit. Return value is an array [value,unit]
  function doAutoScale(num, strUnit) {
    // remove prefix (means calc base unit and value)
    var baseUnitInfo = getExponentAndBaseUnitFromUnit(strUnit)
    var baseValue = num * Math.pow(10, baseUnitInfo[0])
    // calc scaled value and prefixed unit on base values
    var autoScaleExponent = getAutoScaleExponent(baseValue)
    var autoScalePrefix = getPrefixFromExponent(autoScaleExponent)
    var autoScaleValue = baseValue * Math.pow(10, -autoScaleExponent)
    var autoScaleUnit = autoScalePrefix+baseUnitInfo[1]
    return [autoScaleValue, autoScaleUnit]
  }

  function getAutoScaleExponent(num) {
    var floatVal = 0.0
    if(typeof num === "string") {
      floatVal = parseFloat(num)
    }
    else {
      floatVal = num
    }
    floatVal = Math.abs(floatVal)
    var exponent = 0
    // a zero value does not get a prefix
    if(floatVal < 1e-15*autoScaleLimit)
      exponent = 0
    else if(floatVal < 1e-12*autoScaleLimit)
      exponent = -12
    else if(floatVal < 1e-9*autoScaleLimit)
      exponent = -9
    else if(floatVal < 1e-6*autoScaleLimit)
      exponent = -6
    else if(floatVal < 1e-3*autoScaleLimit)
      exponent = -3
    else if(floatVal > 1e3*autoScaleLimit)
      exponent = 3
    else if(floatVal > 1e6*autoScaleLimit)
      exponent = 6
    else if(floatVal > 1e9*autoScaleLimit)
      exponent = 9
    else if(floatVal > 1e12*autoScaleLimit)
      exponent = 12
    return exponent
  }

  function getPrefixFromExponent(exponent) {
    var str = ""
    switch(exponent) {
    case -12:
      str="p"
      break
    case -9:
      str="n"
      break
    case -6:
      str="µ"
      break
    case -3:
      str="m"
      break
    case 3:
      str="k"
      break
    case 6:
      str="M"
      break
    case 9:
      str="G"
      break
    case 12:
      str="T"
      break
    }
    return str
  }

  function getAutoScalePrefix(num) {
    var exponent = getAutoScaleExponent(num)
    return getPrefixFromExponent(exponent)
  }

  function getExponentAndBaseUnitFromUnit(strUnit) {
    var strPrefix = strUnit.substring(0,1)
    var exponent = 0
    switch(strPrefix) {
    case "p":
      exponent = -12
      break
    case "n":
      exponent = -9
      break
    case "µ":
      exponent = -6
      break
    case "m":
      exponent = -3
      break
    case "k":
      exponent = 3
      break
    case "M":
      exponent = 6
      break
    case "G":
      exponent = 9
      break
    case "T":
      exponent = 12
      break
    }
    var strNewUnit = exponent === 0 ? strUnit : strUnit.substring(1)
    return [exponent, strNewUnit]
  }


  /////////////////////////////////////////////////////////////////////////////
  // Locale settings

  readonly property string localeName: settings.globalSettings.getOption("locale", "en_GB")
  readonly property var locale: Qt.locale(localeName)
  onLocaleNameChanged: {
    ZTR.changeLanguage(localeName);
  }

  function setLocale(newLocale) {
    settings.globalSettings.setOption("locale", newLocale);
  }

  /////////////////////////////////////////////////////////////////////////////
  // Number <-> String conversion helpers

  function ceilLog10Of1DividedByX(realNumberX) {
    return Math.ceil(Math.log(1/realNumberX)/Math.LN10)
  }

  function formatNumber(num, decimals) {
    if(typeof num === "string") //parsing strings as number is not desired
    {
      return num;
    }
    else
    {
      var dec = decimals ? decimals : decimalPlaces
      var strNum = Number(num).toLocaleString(locale, 'f', dec)
      // remove group separators (this is ugly but don't get documented examples to fly here...)
      var groupSepChar = locale.decimalPoint === "," ? "." : ","
      while(strNum.includes(groupSepChar)) {
        strNum = strNum.replace(groupSepChar, "")
      }
      return strNum
    }
  }

  /////////////////////////////////////////////////////////////////////////////
  // Misc settings / status
  readonly property string serverIpAddress: settings.globalSettings.getOption("modulemanagerIp", "127.0.0.1");

  //not saved to settings
  property string currentViewName: "";
  property string currentSelectedStoragePath: "/home/operator/logger"; //default


  /////////////////////////////////////////////////////////////////////////////
  // Vein global status

  // Vein components can be used only after vein is initialized otherwise
  // we get nul-errors accessing components. To get around we wrap components
  // in properties and bind them once vein is up. The property
  // 'entityInitializationDone' is set from main.qml...
  property bool entityInitializationDone: false;
  onEntityInitializationDoneChanged: {
    if(entityInitializationDone)
    {
      adjustmentStatusText = Qt.binding(function() {
        return VeinEntity.getEntity("StatusModule1").INF_Adjusted;
      });
    }
  }

  // adjustment status helpers
  property string adjustmentStatusText: "0"
  readonly property bool adjustmentStatusOk : {
      // To avoid confusion we assume adjusted state as long as vein is not up
      return !entityInitializationDone || parseInt(adjustmentStatusText) === 0
  }

  readonly property string adjustmentStatusDescription : {
      var status = parseInt(adjustmentStatusText)
      var strStatus = "OK"
      if(status !== 0) {
          strStatus = ""
          // see mt310s2d/com5003d / adjustment.h for flags definition
          if(status & 1) {
              strStatus += ZTR["Not adjusted"]
          }
          if(status & 2) {
              if(strStatus !== "") {
                  strStatus += " / "
              }
              strStatus += ZTR["Wrong version"]
          }
          if(status & 4) {
              if(strStatus !== "") {
                  strStatus += " / "
              }
              strStatus += ZTR["Wrong serial number"]
          }
      }
      return strStatus;
  }
}