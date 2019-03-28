# Important
* Design your code before implementing it.
* Follow correct coding conventions and avoid breaking Oryol's own conventions.
* Put all your documentations in the [Documentations](Documentations/) folder.
* Make your components minimal but efficient (avoid boilerplate code)

## Code Examples
To make better use of the features of the different `World` components, some code examples are here provided.

#### Fetching Entities
```c
// get all entities on the tile at the given world position
auto result = Game::World->GetComponent<Level>()->GetEntities(myPosition);
```

```c
// get all entities on targeted tile AND with given component type
// in this case, the result will only contain explorers
auto result = Game::World->GetComponent<Level>()->GetEntities<Explorer>(myPosition);
```

#### Retrieving Tiles
```c
// retrieve the tile that the given world position is most likely corresponding to
auto tile = Game::World->GetComponent<Level>()->GetTile(myPosition);
```

#### Path Requests
```c
// request a path for given entity to some target position
Game::World->GetComponent<PathPlanner>()->RequestPath(targetPos, myEntity);
// or by message request
Game::World->OnReceiveMessage(PathRequest::Create(targetPos, myEntity));
```

#### Accessing Castles
```c
// get a castle entity with given team identifier
auto castle = Game::World->GetComponent<Level>()->GetCastle(myTeam);
```

## Using the ECS module
* **All components must declare `ECSComponentType(MyComponent)` in the header.**
* **All messages must declare `ECSMessageType(MyMessage)` in the header.**
* An entity may only contain one component of each component *type*.
* You may choose between `Update()` and `FixedUpdate()` in your components.
* You may view an `Entity` as only an identifier, the actual content is built by its `BaseComponent` subclasses.
* Be careful when using the `GetAttribute` and `SetAttribute` functions since they do not guarantee that the attribute pointers still are valid.
* Retrieving or removing components that do not exist will yield an error, use `HasComponent<T>()` instead.
* To construct `RefCounted` classes, such as `Message`, use `MyObject::Create(...)`.

### Advanced
* You may select the access level of attributes when they are registered using `Attribute::Access`.
* Reading/writing to an attribute that does not have the correct access level will yield an error.
* You may change the maximum number of entities and components in `ECSConfig`.
* The update rate is of `FixedUpdate()` is by default set to 60 but may be changed in `EntityManager`