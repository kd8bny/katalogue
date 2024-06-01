#include <QCoreApplication>

// #include "SecretDataPopulate.h"

Q_DECL_EXPORT int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    QCoreApplication::setApplicationName(QStringLiteral("katalogue_tests"));

    return 0;
}
