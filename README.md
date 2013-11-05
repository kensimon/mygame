Mygame
------

Now that I'm throwing this on github I should probably explain to the
unsuspecting visitor what this *is*.

This was a very early attempt to learn C++, opengl, boost, multithreading,
autotools, debian packaging, and whatever else I felt like putting in this
project.

It's not very good code, but I learned a lot as I wrote it.

It's just a dumb opengl window with shapes that appear when you right, or
middle click.  You can drag them around and watch them bounce...  There's lots
of keybindings for drawing bounding boxes, turning off gravity, and so forth.

"Architecture" (suspicious quotes)
----------------------------------

Originally I wanted to make a multithreaded collision detection algorithm.
Strictly speaking I did, but it's slower than a single threaded implementation
because all the mutex action going on.  A proper way to do it (if I wanted to
write it again) would be to dispatch a single worker to each processor, and map
the work of detecting collisions for each object to each project according to
something like this pseudocode:

    find all objects where (objnum % num_processors == current_processor)

(ie. "shard" the objects by processor)

And just sest each processor loose on doing collision detection between its own
objects and the rest of them.

Either way it's totally trivial because I'm not *doing* anything once I detect
collisions, other than changing the color.

Author:

Ken Simon <ninkendo@gmail.com>
