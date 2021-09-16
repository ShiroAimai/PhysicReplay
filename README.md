# PhysicReplay
End of term project for High Level Programming university course for my master's degree.

##About
PhysicReplay is a physic simulation of actor instances, either spheres or cubes, in which, given a sample rate per second,
a Manager extracts and saves data from all the instances implementing a specific **IReplayableActor** interface.
These entities will contain a **EntityReplayComponent** which purpose is to be a cache in between
the model data and the Manager.

The manager also manages a timer, when the time ends the manager set all the tracked
instances state to be **REPLAY_DRIVEN**, and begins to set the previously tracked data back
to instances in a strictly sequential order. During the **REPLAY_DRIVEN** phase all instances
will re-execute all the physic and movements they did back during the previous **ENTITY_DRIVEN** phase,
exactly like a replay.

Data are tracked using the Unreal Engine Property System (or Reflection System) using a string with a set of EntityReplayComponent Property Names, separated by a ",".
EntityReplayComponents are able to get properties given their name, so the class is able to perform actions
on a property just given its name.

This approach is useful 
to limit getter/setter of all the properties in EntityReplayComponent to two general functions.
This helps to decouple code between the EntityReplayComponent and its clients, because clients does not need direct 
access to properties.

This approach is most useful to quickly change the set of tracked properties without writing
specific code for properties.