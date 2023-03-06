# Ubisoft Next Submission Write Up

Disclaimer: I am writing this 20 minutes before the deadline so I cannot be too detail with my code base

## Code Setup

Tested For Microsoft Visual Studio 2019

## Features

- Basic 3rd person camera perspectives
- Entity Component System
- Basic 3d renderer
- Basic 2d physics
- Simple 3d physics (I've tried to implement this but the workload and complexity was too much the basic skeleton is under /Systems/Physics3D)

## Overall code structure

The overall design of the code base is heavily inspired by the following article [link](https://www.david-colson.com/2020/02/09/making-a-simple-ecs.html). The Code Base consist of entities which are containers for components which hold the raw
data for which the different Systems operate on.

## Known Bugs
When you kill an enemy their collider isn't actually removed they turn invisible and might kill you it makes the game 10 times harder (It's a bug not a feature!), you can also notice the bug in the submission video.

