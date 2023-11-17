.. _introduction_doc:

Introduction
=============

``HepEmShow`` is an application for simulating electromagnetic (EM) shower in a configurable simplified sampling calorimeter.
While the EM shower is simulated by the same algorithm and physics used today for detector simulation in High Energy Physics (HEP),
including the Large Hadron Collider (LHC) experiments such as ATLAS or CMS, the entire simulation is kept very lightweight and simple
in order to enhance the clarity and transparency of the underlying computing flow and algorithms.

The ``Geant4`` :cite:`agostinelli2003geant4,allison2006geant4,allison2016recent` simulation toolkit is the main workhorse for such simulations
today due to the flexibility provided by its carefully design interfaces, powerful geometry description, navigation and comprehensive physics
modelling capabilities. While from the applications point of view these properties are advantageous (e.g. making possible to handle even the
most complex simulation problems some of which listed above), they might cause considerable difficulties when the goal is to identify and
investigate the underlying computing flow and algorithms. This is due to the simple fact that a relatively large fraction of the ``Geant4``
codebase is involved even in a simple simulation application due to the generic internal framework of the toolkit. It already requires substantial
effort and time to become familiar enough with the toolkit for application development but even more effort and significant expertise are need
when the goal is to identify and understand the underlying computing flow and algorithms. This can cause difficulties or even pose barriers
especially when non-simulation experts would like to take some ``Geant4`` based simulation as the application target of their own technology,
new techniques, solution, etc.. Moreover, applying the new technology to the complete, final simulation target usually has significant cost and
risk (see above) that can be largely reduced by preceding with a feasibility study on a simpler, more compact but realistic simulation, i.e.
having the key algorithmic properties identical to the native ``Geant4`` based simulation.

``HepEmShow`` was developed exactly with this goal in mind: to provide a compact, ``Geant4`` like but ``Geant4`` independent, significantly
simpler simulation application having only the necessary components but all of them available locally. More details on the main components of the
simulation application can be found in :ref:`The components of the simulation <simulation_components_doc>` section.
