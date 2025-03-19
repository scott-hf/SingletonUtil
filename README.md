
# SD Singleton Util - Destroyer of hard references

A blueprint-exposed, C++ singleton utility **plugin** for **Unreal Engine**, designed to cache and retrieve singleton instances of actors, components interfaces, and global objects.

## TLDR
- Stop calling "get actor of class" and caching it as a variable
- Remove hard references between blueprints

 ![singleton util gif v3](https://github.com/user-attachments/assets/96750a36-bbe2-4f46-8d18-a25f209833f6)


## Intent
- De-couple more, cache easier, and remove clutter from variables in your blueprints - while remaining performant
- Call singleton functions for any actor, component, or interfaces without having to worry about whether it exists. it can auto-create classes where needed

## Usage

### 1. Get Singleton Actor
Retrieve a singleton actor
- `MyActorClass`: The actor class to retrieve as a singleton.
- `bCreateIfMissing`: If `true`, creates a new instance if none exists.
- 
### 2. Get Singleton Component
Retrieve a singleton component
- `CompnoentClass`: The component class to retrieve as a singleton.
- `bCreateIfMissing`: If `true`, creates a new instance if none exists.

### 3. Get Singleton Interface
Retrieve a singleton instance of a specific interface.
- `MyInterfaceClass`: Interface class to retrieve as a singleton.
- `OutObject`: Receives the object implementing the interface.

## Installation

1. Clone or download this repository into your project's `Plugins` folder.
2. Enable the **Singleton Util Plugin** in your project's plugin settings.

## Special Thanks

Thank you to avigrail, who helped provided examples on how to declare interface specific return values, among other things


## ----------------
## Additional Info

## Additional Features

- Global UObject Registry: Register and retrieve global UObjects with optional identifiers.
- Derived Class Caching: Efficiently cache derived classes for retrieval.
- Debug Tools: Inspect the current state of singleton caches for actors and objects.


## [[Advanced]] Search Parameters for finding your UObject. Usually default search params work but just in case, here are some options
![image](https://github.com/user-attachments/assets/0671dcee-33a3-4466-90e8-998e2fdeb594)

## Debugging

- Object Cache Snapshot: Retrieve a snapshot of the current object cache.
- Actor Cache Snapshot: Inspect the current actor cache for debugging.
- Interface Cache Snapshot: Inspect the current interface cache for debugging.

## All blueprint-exposed functions accessible from SD Singleton Subsystem
![image](https://github.com/user-attachments/assets/557a52e3-4963-468c-9149-55a947d9e179)
