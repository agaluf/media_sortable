---=== Reason behind the module ===---

By default, Media widget provides a sortable table of files.
Such an interface is unintuitive and crude. To improve the backend user experience,
this module was written that combines the Plupload, jQueryUI and Media
to provide a simple drag and drop User Interface directly in the form.

The module is only compatible with Media 2.x

---=== Dependencies ===---

https://www.drupal.org/project/jquery_update
https://www.drupal.org/project/media

---=== Installation ===---

Step 1: Download and install the module as usual
Step 2: Apply a media patch #4 here: https://www.drupal.org/node/2474639
Step 2: Create a field of type file or image and set its widget to Media Sortable.

That's it, you're done.

Whenever you have a field with multiple values, the standard Drupal draggable table
will be replaced with an unordered list that you can style as you please.
It even comes with a (granted, ugly) trash field where you can drag your items to remove them.

---=== TODOs ===---

At the moment, the edit option is not active, because I had no need of it initially.
It is coming eventually, though.
