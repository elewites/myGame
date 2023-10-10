# How to Play 

## Controls
- `W` key to move up.
- `S` key to move down.
- `A` key to move left.
- `D` key to move right.
- `P` key to pause.
- `Left-mouse click` to shoot. 

## Run the Game
1. Download [Release.zip](Release.zip)
2. Unzip the folder.
3. Run `Game02.exe` file.

## Project Structure

[bin](bin) folder contains executable files built with Visual Studio. 

[gameSpecifications](gameSpecifications) folder has fonts used in game, a [game Config](gameSpecifications/config.txt) file and 
a [program specifications](gameSpecifications/programSpecifications.txt) file. Read program specifications if you want to understand how the game config file
specifies the configurations for the game. In short, game config specifies the size, shape, and speed of entities in the game. 
Game config is then read by game upon initialization. 

[libraries](libraries) contains the dependencies that were used to build this program. Mainly, SFML library.

[src](src) folder contains the source code. 

## Game Design

This game was designed using an Entity-Components-System (ECS) approach.

**Entities**: In ECS, entities are the core elements of the game, representing objects such as players, enemies, projectiles, and interactive elements. They serve as containers for various components.
- My [Entity](src/model/Entity.h) class encapsulates data related to an entity. In my game, three specific types of entities come to life, `Enenmy`, `Player`, `Bullet`.
  The type of entity is set by the `m_tag`. Overall, the Entity class is simple, it stores whether an entity is alive, entity's id, and tag (type of Entity). Entity also has pointers to the components it may possess. 

**Components**: Components are data containers that specify attributes or behaviors of entities. They can store information about an entity's position, rendering, physics, or AI behavior, among others. Entities can have multiple components. 
- My [Components](src/model/Components.h) class encapsulates data related to components. The components that I have created for this game are:
  1. `CTransform`: stores data regarding where an entity is and where it is going
  2. `CShape`: stores data related to the shape of an entity
  3. `CCollision`: stores data related to the radius of collision of an entity
  4. `CScore`: stores the score associated with an entity.
  5. `CLifeSpan`: stores data related to life span of an entity (how many game frames an entity must live).
  6. `CInput`: if entity has CInput component, user can control entity with keyboard keys.
 
  **Systems**: ECS employs systems to manage and process entities based on their components. Each system specializes in a particular aspect of game logic, such as rendering, physics simulation, collision detection, or artificial intelligence. Systems work independently and interact with entities possessing the required components.
  My [Game](src/model/Game.h) class encapsulates code related to the game systems. The systems I created for this game are:
  1. `sMovement`: handles logic related to the movement of entities in the game
  2. `sUserInput`: takes care of reading user input and responding appropriately to this input 
  3. `sLifeSpan`: handles lifespan of entities. 
  4. `sRender`: draws entities to the game window. 
  5. `sEnemySpawner`: handles spawning of enemies.
  6. `sCollision`: handles collisions between enemies.
 
### Entity Manager
The [EntityManager](src/model/EntityManager.h) class serves as a central component within my game engine. It is responsible for managing all game entities, controlling their lifecycle, and providing a structured way to interact with them. The key methods of this class are update, addEntity, and removeDeadEntities, where lifecycle of entities are handled accordingly. 









