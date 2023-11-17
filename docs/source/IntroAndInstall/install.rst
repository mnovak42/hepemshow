.. _install_doc:

Build and Install
==================

The repository provides two applications. The main ``HepEmShow`` simulation and the auxiliary ``HepEmShow-DataGeneration`` applications. The minimum requirement to build and execute the ``HepEmShow`` simulation application with its default material
configuration is ``G4HepEm`` :cite:`g4hepem`.

Quick start
------------

It is strongly recommended to read the rather short `Details`_ section below!


To clone, configure, build and install the required ``G4HepEm`` under ``YOUR_G4HEPEM_INSTALL_LOCATION``::

    $ git clone https://github.com/mnovak42/g4hepem.git
    Cloning into 'g4hepem'...
    ...
    $ mkdir g4hepem/build && cd g4hepem/build
    $ cmake ../ -DG4HepEm_GEANT4_BUILD=OFF -DCMAKE_INSTALL_PREFIX=YOUR_G4HEPEM_INSTALL_LOCATION
    -- The C compiler identification is ...
    ...

    -- Configuring done
    -- Generating done
    -- Build files have been written to: ...
    $ make build
      ...
    $ make install
      ...

Then to clone, configure and build the ``HepEmShow`` simulation::

    $ git clone https://github.com/mnovak42/hepemshow.git
    Cloning into 'hepemshow'...
    ...
    $ mkdir hepemshow/build && cd hepemshow/build
    $ cmake ../ -DG4HepEm_DIR=YOUR_G4HEPEM_INSTALL_LOCATION/lib/cmake/G4HepEm/
    -- The C compiler identification is ...
    ...

    -- Configuring done
    -- Generating done
    -- Build files have been written to: ...
    $ make build
      ...

Then executing the simulation reports the default configuration and performs the corresponding EM shower simulation::

  $ ./HepEmShow

    === HepEmShow input parameters:
        --- Geometry configuration:
            - number-of-layers      : 50
            - absorber-thickness    : 2.3 [mm]
            - gap-thickness         : 5.7 [mm]
            - transverse-size       : 400 [mm]
        --- Primary and Event configuration:
            - primary-particle      : e-
            - primary-energy        : 10000 [MeV]
            - number-of-events      : 1000
            - random-seed           : 1234
        --- Additional configuration:
            - g4hepem-data-file    : ../data/hepem_data.json
            - run-verbosity        : 1
    --- EventLoop::ProcessEvents: starts simulation of N = 1000 events...
         - starts processing #event = 100
         - starts processing #event = 200
         - starts processing #event = 300
         - starts processing #event = 400
         - starts processing #event = 500
         - starts processing #event = 600
         - starts processing #event = 700
         - starts processing #event = 800
         - starts processing #event = 900
         - starts processing #event = 1000
    --- EventLoop::ProcessEvents: completed simulation within t = 35.3206 [s]

    --- Results::WriteResults ----------------------------------
    Absorber: mean Edep = 6722.95 [MeV] and  Std-dev = 309.636 [MeV]
    Gap     : mean Edep = 2571.75 [MeV] and  Std-dev = 118.507 [MeV]

    Mean number of gamma       4457.043
    Mean number of e-          7957.899
    Mean number of e+          428.922

    Mean number of e-/e+ steps 36097
    Mean number of gamma steps 40436.2
    ------------------------------------------------------------


.. _instal_details_doc:

Details
-------------

The ``HepEmShow`` application provides a **minimalist and simplistic** implementation of a ``Geant4`` like but ``Geant4`` independent HEP style EM shower simulation in a configurable simplified sampling
calorimeter geometry (see more at the :ref:`Geometry <geometry_doc>` section).

This is possible by fully **relying on the physics** simulation component **provided by the** ``G4HepEm`` **R&D project** :cite:`g4hepem`, that makes available the standard :math:`e^-/e^+` and :math:`\gamma`
EM physics simulation part of ``Geant4`` (i.e. the one used by the LHC experiments such as ATLAS or CMS for detector simulations) with a ``Geant4`` independent run-time component. While the ``G4HepEm`` run-time
functionalities are independent from ``Geant4`` they require some application specific (i.e. material and secondary production cut dependent) physics related data. All these data are generated during the
initialisation of ``G4HepEm`` relying strongly and depending on ``Geant4``. However, it's possible to export the already initialised state of ``G4HepEm`` to a file and then re-initialise ``G4HepEm`` solely
from such a file, i.e. skipping the ``Geant4`` dependent data generation part this way. Moreover, as initialisation, data definition and run-time functionalities are perfectly separated in ``G4HepEm``
and it can optionally be built without the ``Geant4`` dependent initialisation part, ``G4HepEm`` offers a ``Geant4`` like but ``Geant4`` independent standard EM physics component for simulations,
which is exploited in the ``HepEmShow`` simulation.

A ``G4HepEm`` state, corresponding to the default material (and production cut) configuration of the ``HepEmShow`` application, is already available in the repository (in the form of the ``data/hepem_data.json`` file).
This can be used to run a ``HepEmShow`` simulation application with its default material and production cut configuration that is very likely suitable for many studies. See the instructions below on how
:ref:`To build the HepEmShow simulation <build_simulation_only>` to achieve this.

As mentioned above, the ``G4HepEm`` state depends on the material (and production cut) configuration of the detector geometry. Therefore, if one would like to change the materials (and/or production cut) of the ``HepEmShow``
simulation geometry (see more at :ref:`Geometry <geometry_doc>` section), the corresponding ``G4HepEm`` state file needs to be generated and used in the ``HepEmShow`` simulation instead of the provided default ``data/hepem_data.json``.
In order to make this possible, the auxiliary data ``HepEmShow-DataGeneration`` application is also available in the repository. However, as the ``G4HepEm`` state generation requires its ``Geant4`` dependent initialisation
component, one needs to have a complete, ``Geant4`` dependent build of ``G4HepEm``. Moreover ``HepEmShow-DataGeneration`` application constructs and pre-initialises a ``Geant4`` geometry in order to provide the ``Geant4``
state required to start a physics initialisation that will be done by ``G4HepEm`` for generating its state. Therefore, the ``HepEmShow-DataGeneration`` application itself is ``Geant4`` dependent while requires a
``Geant4`` dependent ``G4HepEm`` build. See the instructions below on how :ref:`To build the HepEmShow-DataGeneration as well <build_simulation_only>` if you would like to change the material configuration of the simulation.

.. note:: Other than material and secondary production threshold (or cut) configuration changes, e.g. number of layers, absorber/gap thickness, etc., do not require to generate the ``G4HepEm`` state file as physics
   is independent from them. Moreover, several ``G4HepEm`` state files, corresponding to different material/cut configurations, can be generated and shared. These can be used then to run the simulation with
   any of those material/cut configurations.


----

.. _build_simulation_only:

To build the ``HepEmShow`` simulation:
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Build and install **first the** only **dependence**,  a ``Geant4`` independent version of ``G4HepEm``:

1. Clone the ``G4HepEm`` repository from https://github.com/mnovak42/g4hepem.git::

     $ git clone https://github.com/mnovak42/g4hepem.git
     Cloning into 'g4hepem'...
     ...

2. Create a build directory, change to that and configure the build by setting

   - ``-DG4HepEm_GEANT4_BUILD`` to ``OFF`` don't build the ``Geant4`` dependent part
   - ``-DCMAKE_INSTALL_PREFIX`` to ``YOUR_G4HEPEM_INSTALL_LOCATION`` (use your own, preferred location here)

   ::

     $ mkdir g4hepem/build && cd g4hepem/build
     $ cmake ../ -DG4HepEm_GEANT4_BUILD=OFF -DCMAKE_INSTALL_PREFIX=YOUR_G4HEPEM_INSTALL_LOCATION
     -- The C compiler identification is ...
     ...

     -- Configuring done
     -- Generating done
     -- Build files have been written to: ...

3. Build and install ``G4HepEm``::

     $ make build
       ...
     $ make install
       ...

``G4HepEm``, the only requirement of the ``HepEmShow`` simulation, should be ready to be used under ``YOUR_G4HEPEM_INSTALL_LOCATION``.

Building **then the** ``HepEmShow`` **simulation**:

1. Clone the ``HepEmShow`` repository from https://github.com/mnovak42/hepemshow.git::

     $ git clone https://github.com/mnovak42/hepemshow.git
     Cloning into 'hepemshow'...
     ...

2. Create a build directory, change to that and configure the build by setting:

   - ``-DG4HepEm_DIR`` to ``YOUR_G4HEPEM_INSTALL_LOCATION/lib/cmake/G4HepEm/`` or where the ``G4HepEmConfig.cmake`` file is located under your own install location

   ::

     $ mkdir hepemshow/build && cd hepemshow/build
     $ cmake ../ -DG4HepEm_DIR=YOUR_G4HEPEM_INSTALL_LOCATION/lib/cmake/G4HepEm/
     -- The C compiler identification is ...
     ...

     -- Configuring done
     -- Generating done
     -- Build files have been written to: ...

3. Build the ``HepEmShow`` simulation::

     $ make build
       ...

The ``HepEmShow`` executable should then be available in the build directory. Executing it performs a simulation with the default geometry, event, etc. configuration as reported::

 $ ./HepEmShow

   === HepEmShow input parameters:
       --- Geometry configuration:
           - number-of-layers      : 50
           - absorber-thickness    : 2.3 [mm]
           - gap-thickness         : 5.7 [mm]
           - transverse-size       : 400 [mm]
       --- Primary and Event configuration:
           - primary-particle      : e-
           - primary-energy        : 10000 [MeV]
           - number-of-events      : 1000
           - random-seed           : 1234
       --- Additional configuration:
           - g4hepem-data-file    : ../data/hepem_data.json
           - run-verbosity        : 1
   --- EventLoop::ProcessEvents: starts simulation of N = 1000 events...
        - starts processing #event = 100
        - starts processing #event = 200
        - starts processing #event = 300
        - starts processing #event = 400
        - starts processing #event = 500
        - starts processing #event = 600
        - starts processing #event = 700
        - starts processing #event = 800
        - starts processing #event = 900
        - starts processing #event = 1000
   --- EventLoop::ProcessEvents: completed simulation within t = 35.3206 [s]

   --- Results::WriteResults ----------------------------------
   Absorber: mean Edep = 6722.95 [MeV] and  Std-dev = 309.636 [MeV]
   Gap     : mean Edep = 2571.75 [MeV] and  Std-dev = 118.507 [MeV]

   Mean number of gamma       4457.043
   Mean number of e-          7957.899
   Mean number of e+          428.922

   Mean number of e-/e+ steps 36097
   Mean number of gamma steps 40436.2
   ------------------------------------------------------------

----

.. _build_both:

To build the ``HepEmShow-DataGeneration`` as well:
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Due to the reasons explained in the `Details`_ above, building the ``HepEmShow-DataGeneration`` auxiliary application requires ``Geant4``
and a complete ``Geant4`` dependent build of ``G4HepEm``. It is assumed here that an appropriate version of ``Geant4`` is installed on the system
(``geant4-11.1`` is recommended when writing this document) under ``YOUR_GEANT4_INSTALL_LOCATION``.

The procedure is then rather similar to the one shown above with the difference that we need to provide the appropriate ``Geant4`` install location both
when build ``G4HepEm`` as well as building the ``hepemshow`` repository.

So building and installing **first the complete**, ``Geant4`` dependent version of ``G4HepEm``:

1. Clone the ``G4HepEm`` repository from https://github.com/mnovak42/g4hepem.git::

     $ git clone https://github.com/mnovak42/g4hepem.git
     Cloning into 'g4hepem'...
     ...

2. Create a build directory, change to that and configure the build by setting

   - ``-DG4HepEm_GEANT4_BUILD`` to ``ON`` (same as its default value so can be omitted) complete, ``Geant4`` dependent build
   - ``-DGeant4_DIR`` to ``YOUR_GEANT4_INSTALL_LOCATION/lib/cmake/Geant4/`` or where the ``Geant4Config.cmake`` file is located under your own ``Geant4`` install location
   - ``-DCMAKE_INSTALL_PREFIX`` to ``YOUR_G4HEPEM_INSTALL_LOCATION`` (use your own preferred location here)

   ::

     $ mkdir g4hepem/build && cd g4hepem/build
     $ cmake ../ -DG4HepEm_GEANT4_BUILD=ON -DGeant4_DIR=YOUR_GEANT4_INSTALL_LOCATION/lib/cmake/Geant4/ -DCMAKE_INSTALL_PREFIX=YOUR_G4HEPEM_INSTALL_LOCATION
     -- The C compiler identification is ...
     ...

     -- Configuring done
     -- Generating done
     -- Build files have been written to: ...

3. Build and install ``G4HepEm``::

     $ make build
       ...
     $ make install
       ...

The complete, ``Geant4`` dependent ``G4HepEm`` should be ready to be used under ``YOUR_G4HEPEM_INSTALL_LOCATION``.

Building **then both the** ``HepEmShow`` simulation and ``HepEmShow-DataGeneration`` applications:

1. Clone the ``HepEmShow`` repository from https://github.com/mnovak42/hepemshow.git::

     $ git clone https://github.com/mnovak42/hepemshow.git
     Cloning into 'hepemshow'...
     ...

2. Create a build directory, change to that and configure the build by setting:

   - ``-DG4HepEm_DIR`` to ``YOUR_G4HEPEM_INSTALL_LOCATION/lib/cmake/G4HepEm/`` or where the ``G4HepEmConfig.cmake`` file is located under your own install location
   - ``-DGeant4_DIR`` to ``YOUR_GEANT4_INSTALL_LOCATION/lib/cmake/Geant4/`` or where the ``Geant4Config.cmake`` file is located under your own ``Geant4`` install location


   ::

     $ mkdir g4hepem/build && cd g4hepem/build
     $ cmake ../ -DG4HepEm_DIR=YOUR_G4HEPEM_INSTALL_LOCATION/lib/cmake/G4HepEm/ -DGeant4_DIR=YOUR_GEANT4_INSTALL_LOCATION/lib/cmake/Geant4/
     -- The C compiler identification is ...
     ...

     -- Configuring done
     -- Generating done
     -- Build files have been written to: ...

3. Build the ``HepEmShow`` simulation::

     $ make build
       ...

Both the ``HepEmShow`` and the ``HepEmShow-DataGeneration`` executables should then be available in the build directory. Note, that the ``HepEmShow`` simulation application is still as
independent from ``Geant4`` as before while the ``Geant4`` dependent ``HepEmShow-DataGeneration`` application makes possible to generate new ``G4HepEm`` state file that corresponds to
different material configurations of the simulation (see more at the :ref:`Geometry <geometry_doc>` section).
