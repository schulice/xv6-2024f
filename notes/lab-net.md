# E1000 Transmitter-Receiver

Follow the guidance. Do not need to fully read the doc of e1000. Check the behavior of e1000 for MMIO and BufferRing.

For receiver, the buffer ring updating is update to the current next. tail->next is the current buffer need to read. But for transmitter, tail is the current pos to hold the buffer pointer. That is a little confused difference.

Before you cover the ownership of buffer to `net.c`, remember to release the e1000_lock because of callback from it to interactive progress.

> [!TIP]  
> Remember to `kfree` the previous buffer when tx take place of the old buffer.

# `net.c`

Implement the `sys_recv` and `ip_rx`. Declare a queue as the buffer to reverse the `port -> multi buffers` mapping, then you can use the similar circle array to keep some buffer when sys_recv not wakeup.

Sleep and Wakeup to achieve a signal-like primitive that each port can "waiting" some harts. Use address of queue node to make unique chan.

> [!important] 
> `((k = func()) == 0)` is not `((k = func() == 0))`, which take some of my time to find this stupid error, twice! sad...

