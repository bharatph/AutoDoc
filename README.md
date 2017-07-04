AutoDoc
Machine Diagnosis

The concept of a machine diagnoser is that it reduces the workload of a real person cross checking the data and also reduces the error of inference. The error of inference of the machine is reduced gradually as the machine while inferring data asks the feedback of the users and uses that data to improve itself.

The AutoDoc is made in the server and client model. This approach is adopted to reduce the processing cost of the user device and because of the problem of distributing the OpenCV package to the user which is very essential for processing images. Though it is possible to compile and  run both server and client in the same linux box since the data transport is by means of socket. The server is the main processing unit which takes care of the cross referencing and finding the diagnosis which leaves the client to send the medical sample and respond by sending the diagnosis to it.

The data that server accepts is both binary and text data. So we can both send forms or send medical samples which will then be inferred and diagnosed by the server. The medical samples indicated in this paper is inclusive of images(experimental) and documents(highly experimental). Video samples are not supported. 
Although 'AutoDoc' refers to the entire project the client only side is also called as AutoDoc and the server is referred as 'AutoDocd' where the 'd' stands for daemon.

The 'AutoDocd' is intended for use in linux distributions(Refer below to see the tested linux distributions), though it can be compiled to other UNIX systems as well.

The 'AutoDoc' client side is of different implementations(interms of GUI and networking library) for different Operating Systems and CPU Architectures since it is the one that had to run on users machine. It follows a very simple non-authenticative protocol over the sockets to the send and retreive data to and from the server across platforms. The client side of 'AutoDoc' is essentially cross-paltform.

AutoDoc Protocol(RXP)
The RXP is very simple protocol idea that has no authentication(not to be confused with defenseless) and has support for session retainment. It maintains a session-id to identify a active session. This session-id is used when the connection is interupted when a file is being uploaded to the server. This ensures that the uploaded files are not uploaded again when reconnecting.

AutoDocd tested Distributions
Ubuntu(>16.04)
Arch Linux
