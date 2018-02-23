IsEmpty is checked when popping, as I shouldn't be popping an empty queue.  

I tend to just use the int GetSize checks (arrayCounter) instead of using GetSize, but I have IsEmpty check it.  I demonstrate that both work by attempting to Pop the queue
before adding anything to it.

For SOME REASON my configuration settings got changed since starting this project and it stopped making the Z_DELETE folder when I compiled.  I went back and redid the
configuration settings but I am unsure if this may have had a lasting impact on my project's filesize (it might have something to do with this computer automatically using x64 for some reason)