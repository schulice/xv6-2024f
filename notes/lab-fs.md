# Large File

Add a 2 indirect index on 13 in inode data array.

Just do twice what 1 indirect index do.

# Symlink

just create a new symlink file on current path with sympath in current inode data.

Copy the previous direntry lookup function. When go to the next inode. probe the content of next inode then check whether is symlink. This is because symlink can be relative to current parent inode.

When open a file, just simply get the real path from (maybe)symlink path. This is slow but simply. If want more effective, use a multi path stack meta to lookup the sympath.
