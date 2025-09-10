🚧 Under construction 🚧

Generic, ready-to-use plugin framework for developing Qt/QML/C++ desktop applications.

This project targets developers who want to focus more on what their app does rather than how it is wrapped. It relies on three simple yet powerful design patterns:
- [Plugin architecture pattern](https://cs.uwaterloo.ca/~m2nagapp/courses/CS446/1195/Arch_Design_Activity/PlugIn.pdf)
- [Singleton pattern](https://refactoring.guru/design-patterns/singleton)
- [Dependency injection pattern](https://aveuiller.github.io/about_design_patterns-dependency_injection.html)

By combining them, the J-app as I call it (needed a namespace...) provides handy utilities for
- creating new plugins,
- loading them at runtime,
- logging,
- accessing centralized information from accross the application.

It is meant to become a single library against which you can link your project.

### License
This project is licensed under the MIT License.