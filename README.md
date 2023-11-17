<p align="center">  
  <a href="https://hepemshow.readthedocs.io/en/latest/">
    <img src="./docs/source/logo_HepEmShow.png"></a>
</p>


# Introduction & motivation  

The motivation of the project was to provide a **mock-up of a typical** [`Geant4`](https://geant4.web.cern.ch) **simulation application**. Something that preserves the most important algorithmic and computing characteristics of a typical [`Geant4`](https://geant4.web.cern.ch) based particle transport simulation while exposes the underlying main computing flow, algorithms and components in a clear and simple way. The primary goal was to provide a starting point, a sandbox that can be easily utilised even by non-experts for initial testing, trying new technologies, techniques targeting [`Geant4`](https://geant4.web.cern.ch) based simulation applications in order encourage, stimulate and facilitate such activities.     

`HepEmShow` provides a **minimalist, simplistic** but **complete and self contained** implementation of a [`Geant4`](https://geant4.web.cern.ch) like but [`Geant4`](https://geant4.web.cern.ch) independent simulation application for modelling `High Energy Physics`(**HEP**) style  `electromagnetic`(**EM**) `shower`(**SHOW**) in a configurable, simplified sampling calorimeter illustrated below.

<p align="center">
  <img src="./docs/source/IntroAndInstall/figs/calo_layer2.png" alt="drawing" width="500"/>
</p>

Beyond the usual **application specific** simulation **components** (e.g. application geometry or primary particle/event generation description), ``HepEmShow`` **also** includes a minimalist and simplistic but **local implementation of** all necessary, **general** [`Geant4`](https://geant4.web.cern.ch) **kernel components** (e.g. event processing or stepping loops). This makes `HepEmShow` self contained and complete allowing to alter and change the underlying computing flow, components and algorithm at any points.

It must be noted, that while `HepEmShow` is a [`Geant4`](https://geant4.web.cern.ch) independent EM shower simulation, the provided results agree very well to those obtained by the corresponding [`Geant4`](https://geant4.web.cern.ch) based simulation as illustrated below.

<p align="center">
  <img src="./docs/source/IntroAndInstall/figs/fig-Edep-G4-vs-HepEmShow-10Mill.png" alt="drawing" width="500"/>
</p>

# Requirements

The only requirement, that already allows to build and execute the `HepEmShow` simulation application (using the default material configuration of the calorimeter), is [`G4HepEm`](https://github.com/mnovak42/g4hepem). It is actually [`G4HepEm`](https://github.com/mnovak42/g4hepem) that makes possible to develop such a [`Geant4`](https://geant4.web.cern.ch) like but [`Geant4`](https://geant4.web.cern.ch) independent EM shower simulation like `HepEmShow`. See more details in the [**corresponding part**](https://hepemshow.readthedocs.io/en/latest/IntroAndInstall/install.html#details) of the [**Build and Install**](https://hepemshow.readthedocs.io/en/latest/IntroAndInstall/install.html]) section of the [**Documentation**](https://hepemshow.readthedocs.io/en/latest/) or jump to [**Quick start**](https://hepemshow.readthedocs.io/en/latest/IntroAndInstall/install.html#quick-start).
