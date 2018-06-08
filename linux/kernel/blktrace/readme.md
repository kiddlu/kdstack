⁠6.3. Tools

There are a number of tools available to help diagnose performance problems in the I/O subsystem. vmstat provides a coarse overview of system performance. The following columns are most relevant to I/O: si (swap in), so (swap out), bi (block in), bo (block out), and wa (I/O wait time). si and so are useful when your swap space is on the same device as your data partition, and as an indicator of overall memory pressure. si and bi are read operations, while so and bo are write operations. Each of these categories is reported in kilobytes. wa is idle time; it indicates what portion of the run queue is blocked waiting for I/O complete.

Analyzing your system with vmstat will give you an idea of whether or not the I/O subsystem may be responsible for any performance issues. The free, buff, and cache columns are also worth noting. The cache value increasing alongside the bo value, followed by a drop in cache and an increase in free indicates that the system is performing write-back and invalidation of the page cache.
Note that the I/O numbers reported by vmstat are aggregations of all I/O to all devices. Once you have determined that there may be a performance gap in the I/O subsystem, you can examine the problem more closely with iostat, which will break down the I/O reporting by device. You can also retrieve more detailed information, such as the average request size, the number of reads and writes per second, and the amount of I/O merging going on.

Using the average request size and the average queue size (avgqu-sz), you can make some estimations about how the storage should perform using the graphs you generated when characterizing the performance of your storage. Some generalizations apply: for example, if the average request size is 4KB and the average queue size is 1, throughput is unlikely to be extremely performant.

If the performance numbers do not map to the performance you expect, you can perform more fine-grained analysis with blktrace. The blktrace suite of utilities gives fine-grained information on how much time is spent in the I/O subsystem. The output from blktrace is a set of binary trace files that can be post-processed by other utilities such as blkparse.

blkparse is the companion utility to blktrace. It reads the raw output from the trace and produces a short-hand textual version.

The following is an example of blktrace output:
8,64   3        1     0.000000000  4162  Q  RM 73992 + 8 [fs_mark]
8,64   3        0     0.000012707     0  m   N cfq4162S / alloced
8,64   3        2     0.000013433  4162  G  RM 73992 + 8 [fs_mark]
8,64   3        3     0.000015813  4162  P   N [fs_mark]
8,64   3        4     0.000017347  4162  I   R 73992 + 8 [fs_mark]
8,64   3        0     0.000018632     0  m   N cfq4162S / insert_request
8,64   3        0     0.000019655     0  m   N cfq4162S / add_to_rr
8,64   3        0     0.000021945     0  m   N cfq4162S / idle=0
8,64   3        5     0.000023460  4162  U   N [fs_mark] 1
8,64   3        0     0.000025761     0  m   N cfq workload slice:300
8,64   3        0     0.000027137     0  m   N cfq4162S / set_active wl_prio:0 wl_type:2
8,64   3        0     0.000028588     0  m   N cfq4162S / fifo=(null)
8,64   3        0     0.000029468     0  m   N cfq4162S / dispatch_insert
8,64   3        0     0.000031359     0  m   N cfq4162S / dispatched a request
8,64   3        0     0.000032306     0  m   N cfq4162S / activate rq, drv=1
8,64   3        6     0.000032735  4162  D   R 73992 + 8 [fs_mark]
8,64   1        1     0.004276637     0  C   R 73992 + 8 [0]
As you can see, the output is dense and difficult to read. You can tell which processes are responsible for issuing I/O to your device, which is useful, but blkparse can give you additional information in an easy-to-digest format in its summary. blkparse summary information is printed at the very end of its output:
Total (sde):
Reads Queued:          19,       76KiB  Writes Queued:     142,183,  568,732KiB
Read Dispatches:       19,       76KiB  Write Dispatches:   25,440,  568,732KiB
Reads Requeued:         0               Writes Requeued:       125
Reads Completed:       19,       76KiB  Writes Completed:   25,315,  568,732KiB
Read Merges:            0,        0KiB  Write Merges:      116,868,  467,472KiB
IO unplugs:        20,087               Timer unplugs:           0
The summary shows average I/O rates, merging activity, and compares the read workload with the write workload. For the most part, however, blkparse output is too voluminous to be useful on its own. Fortunately, there are several tools to assist in visualizing the data.
btt provides an analysis of the amount of time the I/O spent in the different areas of the I/O stack. These areas are:
Q — A block I/O is Queued
G — Get Request
A newly queued block I/O was not a candidate for merging with any existing request, so a new block layer request is allocated.
M — A block I/O is Merged with an existing request.
I — A request is Inserted into the device's queue.
D — A request is issued to the Device.
C — A request is Completed by the driver.
P — The block device queue is Plugged, to allow the aggregation of requests.
U — The device queue is Unplugged, allowing the aggregated requests to be issued to the device.
btt breaks down the time spent in each of these areas, as well as the time spent transitioning between them, like so:

```
Q2Q — time between requests sent to the block layer

Q2G — how long it takes from the time a block I/O is queued to the time it gets a request allocated for it

G2I — how long it takes from the time a request is allocated to the time it is Inserted into the device's queue

Q2M — how long it takes from the time a block I/O is queued to the time it gets merged with an existing request

I2D — how long it takes from the time a request is inserted into the device's queue to the time it is actually issued to the device

M2D — how long it takes from the time a block I/O is merged with an exiting request until the request is issued to the device

D2C — service time of the request by the device

Q2C — total time spent in the block layer for a request
```

You can deduce a lot about a workload from the above table. For example, if Q2Q is much larger than Q2C, that means the application is not issuing I/O in rapid succession. Thus, any performance problems you have may not be at all related to the I/O subsystem. If D2C is very high, then the device is taking a long time to service requests. This can indicate that the device is simply overloaded (which may be due to the fact that it is a shared resource), or it could be because the workload sent down to the device is sub-optimal. If Q2G is very high, it means that there are a lot of requests queued concurrently. This could indicate that the storage is unable to keep up with the I/O load.
Finally, seekwatcher consumes blktrace binary data and generates a set of plots, including Logical Block Address (LBA), throughput and seek operations per second.
⁠
Example seekwatcher output
Figure 6.2. Example seekwatcher output

All plots use time as the X axis. The LBA plot shows reads and writes in different colors. It is interesting to note the relationship between the throughput and seeks/sec graphs. For storage that is seek-sensitive, there is an inverse relation between the two plots.
Prev