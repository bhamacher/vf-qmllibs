#ifndef MYPLUGIN_H
#define MYPLUGIN_H

#include <QtQml/QQmlExtensionPlugin>

/**
 * @brief The MyPlugin class. Simple qml plugin example.
 */

class VfNetworkManager: public QQmlExtensionPlugin {
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "my.plugin.example/1.0")
public:

    /**
     * @brief init library
     *
     * Call this in main before any qml handled.
     * This will register the qml backend and make
     * it available for the qml parts in this lib.
     *
     * In Detail it creates an object of this class and
     * calls registerTypes() with uri "networksettings"
     *
     */
    static void init();
    /**
     * @brief registerTypes Overrided function that should register all
     * C++ classes exported by this plugin.
     * @param uri           Plugin uri.
     */
    void registerTypes(const char* uri) override;
};




#endif // MYPLUGIN_H
