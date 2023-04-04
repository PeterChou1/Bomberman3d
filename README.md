# Bomberman 3d
3d Bomber man Game written in C++ youtube demo video [link](https://www.youtube.com/watch?v=NvjPOeYqf6w)

# Setup
Tested on Visual Studio 2019

# Features

- Entity Component System
- Basic Sound/UI
- Third person camera
- 2d Physics
- Basic 3d Physics

# Overview

The controls of the game is WASD to move. Space to place bomb. E to throw grenade. The player wins the game by killing every enemy.

# Entity Component System

The core part of the code base is the Entity Component System. Entities are simple bit masks which stores that stores which ever components they own.
Components are simple struct that contain the data the game needs to operate on. Each component type is stored in their own component pool for cache
friendly access. The scene object will serve as a database of components and help us access and manage it. We can create new entities, which returns an EntityID, just a number. And then assign components to each entity

Example of a component (can be any simple data struct):
```c++
struct Bomb {

double BombCrossSize = 5;

double Time = 2 . 5 ;

};
```

Example of adding component to a entity:

```c++
Scene scene ;
EntityID triangle = scene.NewEntity () ;
Transform∗ pTransform = scene . Assign<Transform >( triangle ) ;
```


All System in the code base is a subclass of the System class located under Sys-

tems/Systems.h. Systems have two methods they must deﬁne Update and Ren-

der which will run in the Update and Render phases in our game respectively.

To iterate over selected components a custom iterator called the SceneIterator

class is deﬁned for us.

Example of subclassing a System:

```c++
class MouseSystem : public virtual System
{
public:
  MouseSystem( Scene& scene , Mouse∗ mouse ) :
  System( s c ene ) , Mouse (mouse ) {}
  void Update ( f loat deltaTime ) o v e r r i d e ;
  void Render ( ) o v e r r i d e {}
  ˜MouseSystem( ) o v e r r i d e = default ;
private:
  Mouse∗ Mouse{ n u l l p t r } ;
};
```


Example of Iterating over components

```c++
void SoundSystem::Update ( float deltaTime ) {

  int collisonTransform [ ] = { GetId<SoundEffect >() };
  const auto soundIterator = SceneIterator( SystemScene , collisonTransform , 1 ) ;
  for ( const EntityId soundID : soundIterator )
  {
    auto soundEf f e c t =
    SystemScene . Get<SoundEf fect >(soundID ) ;
    App : : PlaySound ( soundEf f e c t−>Soundf i l e ) ;
    SystemScene . De s t royEnt i ty ( soundID ) ;
  }
}
```

In the above class we pass in a list of component indexs via GetId to create our
custom Iterator the Iterator then allows us to traverse the entire Scene to ﬁnd
all entities with the SoundEﬀect component
Once you deﬁned and Instantiate a System you must register the System with
the allSystem object the allSystem objects will iterate through all Systems and
call their Render and Update function respectively. The order matters if you
have a System which depended on another Systems calculation you must regis-
ter the dependent system after.

# Physics

2d and 3d physics are handle by the Physics2D and Physics3D system respectively.
I had originally intended to implement a full 3d rigid body simulation. However
due to time constraints It would be feasible to ﬁnished in time. So I decided
to integrate my already existing 3d physics code that I wrote into the grenade
feature. Had I had more time the next step would have been to implement narrow phase
collision using GJK algorithmn and then implement collision response.

References

Online resources that was helpful during the coding of this project

• https://www.youtube.com/watch?v=ih20l3pJoeU

• https://www.david-colson.com/2020/02/09/making-a-simple-ecs.html

• https://blog.winter.dev/2020/designing-a-physics-engine/

• http://www.cs.cmu.edu/ baraﬀ/sigcourse/index.html

