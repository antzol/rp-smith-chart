#ifndef CONSTANTS_H
#define CONSTANTS_H

constexpr double PI = 3.14159265;
constexpr double epsilon = 1e-5;

enum class ConnectionType {
    Series,
    Parallel
};

enum class ComponentType {
    Inductance,
    Capacitor
};


#endif // CONSTANTS_H
