
# Singleton Util Plugin

A Blueprint-compatible singleton utility for Unreal Engine, designed to manage and retrieve singleton instances of actors, interfaces, and global objects within a project. This plugin provides an efficient way to ensure only one instance of a given actor or interface exists within the world context, with optional caching and customizable search parameters.

## Intent
- De-couple more, cache easier, and remove clutter from variables in your blueprints - while remaining performant
- Call singleton interface functions from c++, without caring about BP implementation. IE: set up a c++ interface, create a BP object with that interface in the world, and call it from c++
- Flexibility for ease of use OR following best practices. Best practices state to de-couple your blueprints and singleton-based interface calls enable that. But if you're just trying to get shit done, call "Get Singleton Actor" - you'll have a direct reference to the actor still, but you won't have to cache it locally in every blueprint that needs to talk to it.

## Blueprint Library Functions
![image](https://github.com/user-attachments/assets/1b9582c5-505c-4f20-83c3-c313a5453481)

## All blueprint-exposed functions accessible from SD Singleton Subsystem
![image](https://github.com/user-attachments/assets/557a52e3-4963-468c-9149-55a947d9e179)

## Notes

- All C++, nothing in the content folder
- All functions exposed to blueprint. Use it in C++ or blueprint - your choice
- The two most commonly used functions are simplified and expose via blueprint function library. To see all functions, retrieve the subsystem.

## Features

- Get Singleton Actor: Retrieve or create a singleton instance of an actor.
- Get Singleton Interface: Retrieve a singleton instance of an interface class.
- Global UObject Registry: Register and retrieve global UObjects with optional identifiers.
- Derived Class Caching: Efficiently cache derived classes for retrieval.
- Debug Tools: Inspect the current state of singleton caches for actors and objects.


## [[Advanced]] Search Parameters for finding your UObject. Usually default search params work but just in case, here are some options
![image](https://github.com/user-attachments/assets/0671dcee-33a3-4466-90e8-998e2fdeb594)

## Installation

1. Clone or download this repository into your project's `Plugins` folder.
2. Enable the **Singleton Util Plugin** in your project's plugin settings.

## Usage

### 1. Get Singleton Actor
Retrieve a singleton actor within the current world context.
- `WorldContextObject`: Reference to the current world.
- `MyActorClass`: The actor class to retrieve as a singleton.
- `bCreateIfMissing`: If `true`, creates a new instance if none exists.

### 2. Get Singleton Interface
Retrieve a singleton instance of a specific interface.
- `WorldContextObject`: Reference to the current world.
- `MyInterfaceClass`: Interface class to retrieve as a singleton.
- `OutObject`: Receives the object implementing the interface.

### 3. Register and Retrieve Global Objects
Register a global object to make it easily accessible across contexts.
Retrieve a global object by class and optional identifier.

### 4. Advanced Search Parameters
Use `FSD_SingletonSearchParams` for refined search criteria for finding interface singleton objects, such as filtering by object type, name, or actor-only objects.

## Debugging

- Object Cache Snapshot: Retrieve a snapshot of the current object cache.
- Actor Cache Snapshot: Inspect the current actor cache for debugging.
- Interface Cache Snapshot: Inspect the current interface cache for debugging.

## Special Thanks

Thank you to avigrail, who helped provided examples on how to declare interface specific return values, among other things
