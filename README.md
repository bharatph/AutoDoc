# AutoDoc
#### Machine Diagnosis

The concept of a machine diagnoser is that it reduces the workload of a real person cross checking the data and also reduces the error of inference. The error of inference of the machine is reduced gradually as the machine while inferring data asks the feedback of the users and uses that data to improve itself.

The only problem that can arise in a self learning machine-diagnoser is that when feeded with bogus feedback it will learn that too and can provide people with unrealiable data. This can be mitigated by making the machine-daignoser from time to time read and update it's knowlege from a realiable book or any other data source such as the Wikipedia.

#### About
This project is not intended to replace Doctors, instead it is used as tool for predicting disease which can be used by doctors.

The AutoDoc is made in the server and client model. This approach is adopted to reduce the processing cost of the user device and because of the problem of distributing the OpenCV package to the user which is very essential for processing images. This also gives offers us many possibilities, for example client can be run from Android phone, iOS device, and for almost every device that supports web. Though it is possible to compile and  run both server and client in the same linux box since the data transport is by means of socket. The server is the main processing unit which takes care of the cross referencing and finding the diagnosis which leaves the client to send the medical sample and respond by sending the diagnosis to it. Moreover processing of content and making sense of them does require a dedicated unit.

But this does not force the user to use the Authenticated AutoDoc servers. AutoDoc can be both centralized and decentralized. Users have the option to install the server on their computer or they can use the servers which is established by hospitals and other health organizations.

AutoDoc is designed to work with Lazy Protocol(https://github.com/bharatph/lp), which offers anonymity and session management which is fundamental to AutoDoc. Since the data fed to AutoDoc is serious, AutoDoc does not collect user information rather it gives a temporary key to the client which will be expired after some time. The user can resume operations at his leisure using the temporary key. Usually the client does this job and for the users it will seem almost transparent.

The data that server accepts is both binary and text data. So we can both send forms or send medical samples which will then be inferred and diagnosed by the server. The medical samples indicated in this paper is inclusive of images(highly experimental) and documents(experimental). Video samples are not supported.

Although 'AutoDoc' refers to the entire project the client only side is also called as AutoDoc and the server is referred as 'AutoDocd' where the 'd' stands for daemon. A convention derived from POSIX.

The 'AutoDocd' is intended for use in linux distributions(Refer below to see the tested linux distributions), though it can be compiled to other UNIX systems as well.

The 'AutoDoc' client side is of different implementations(interms of GUI and networking library) for different Operating Systems and CPU Architectures since it is the one that had to run on users machine. It follows a very simple non-authenticative protocol over the sockets to the send and retreive data to and from the server across platforms. The client side of 'AutoDoc' is essentially cross-paltform.

If the client is a browser, the LP can be tunneled through http or https.

#### Technology Used
AutoDoc is built using technologies like C++ and CMake. CMake is a build file generator which plays a vital role in porting AutoDoc to numerous Operating System. The reason for choosing C++ is because, it gives a flexibility(compile time code switching) that any other language fails to give and also due to the reason that it blazingly fast and has compilers which supports system that are now considered antique.

Since AutoDoc is based on C++ and CMake, it can be ported to numerous chipsets under a variety of Operating Systems.

#### User Interface

AutoDoc is designed with one thing in mind which is efficiency interms of simplicity. The GUI provided with AutoDoc was made to interact with the user in a efficient way, thus provides the main operations in starting window and advanced options such as changing the server address and port, etc in 'Advanced Settings' under the File Menu, which is not shown to the user by default.

There are other ways the user can interact with AutoDoc. One such example is the Zero UI. Users can interact with AutoDoc just through voice commands.

#### More about LP
LP is very simple protocol idea that has no authentication(not to be confused with defenseless) and has support for session retainment. It maintains a session-id to identify a active session. This session-id is used when the connection is interupted when a file is being uploaded to the server. This ensures that the uploaded files are not uploaded again when reconnecting and holds connection information for a time period, so the users can resume when they go offline unexpectedly.

The LP used by AutoDoc is open source software, which can be obtained from the repo https://github.com/bharatph/lp

#### Practicality issues
The most difficult part in the construction of AutoDoc is good inference of data in optimal time. This one particular factor has a very large effect on the user experience.

#### AutoDocd tested Distributions
Ubuntu(>16.04)
Arch Linux
