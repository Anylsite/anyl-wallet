# Contributing to Anyledger wallet
We love your input! We want to make contributing to this project as easy and transparent as possible, whether it's:

- Reporting a bug
- Discussing the current state of the code
- Submitting a fix
- Proposing new features

## We Develop with Github
We use github to host code, to track issues and feature requests, as well as accept pull requests.

## We Use [Github Flow](https://guides.github.com/introduction/flow/index.html), So All Code Changes Happen Through Pull Requests
Pull requests are the best way to propose changes to the codebase (we use [Github Flow](https://guides.github.com/introduction/flow/index.html)). We actively welcome your pull requests.
Before you start writing the code, please make yourself familiar with [JPL Coding standard][1] and [The power of ten][2]. Even though not all rules from the standard will be enforced, it will give you a good idea of what is expected from the code before it can be merged.

1. Fork the repo and create your branch from `master`.
2. If you've added code that should be tested, add tests.
3. If you've changed APIs, update the documentation.
4. Ensure the test suite passes.
5. Make sure your code lints.
6. Issue that pull request!

All pull requests require the code to be reviewed and approved by other person on the team.

## Red flags

### Compiler warnings
The CI compiler has all warnings enabled and will fail if there is one. Do not silence the warnings with `#pragma` or similar. If you feel there's a reason to supress the warning, please discuss the issue with project maintainer. (Also see: JPL-LOC1.2)

### Dynamic memory allocation
As we are targeting embedded platforms, there shall be no dynamic memory allocation after the task is initialized. (Also see: JPL-LOC2.5)

### No asserts in your code
Use static and dynamic asserts as a sanity checks. It's always better to fail early. (Also see: JPL-LOC3.16)

### Changes to the build subsystem
Please do not change the build/CI configuration without discussing the issue with the team. This also applies to any requirements or additional libraries.


## Any contributions you make will be under the MIT Software License
In short, when you submit code changes, your submissions are understood to be under the same [MIT License](http://choosealicense.com/licenses/mit/) that covers the project. Feel free to contact the maintainers if that's a concern.

## Report bugs using Github's [issues](https://github.com/briandk/transcriptase-atom/issues)
We use GitHub issues to track public bugs. Report a bug by [opening a new issue](); it's that easy!

## Write bug reports with detail, background, and sample code
[This is an example](http://stackoverflow.com/q/12488905/180626) of a bug report I wrote, and I think it's not a bad model. Here's [another example from Craig Hockenberry](http://www.openradar.me/11905408), an app developer whom I greatly respect.

**Great Bug Reports** tend to have:

- A quick summary and/or background
- Steps to reproduce
  - Be specific!
  - Give sample code if you can. [My stackoverflow question](http://stackoverflow.com/q/12488905/180626) includes sample code that *anyone* with a base R setup can run to reproduce what I was seeing
- What you expected would happen
- What actually happens
- Notes (possibly including why you think this might be happening, or stuff you tried that didn't work)

People *love* thorough bug reports. I'm not even kidding.

## Use a Consistent Coding Style
TBD: there'll be a requirement to pass the linter before merging the PR

## License
By contributing, you agree that your contributions will be licensed under its MIT License.

## References
This document was adapted from the open-source contribution guidelines for [Facebook's Draft](https://github.com/facebook/draft-js/blob/a9316a723f9e918afde44dea68b5f9f39b7d9b00/CONTRIBUTING.md)
[JPL Coding standard](https://lars-lab.jpl.nasa.gov/JPL_Coding_Standard_C.pdf)

[1]: https://lars-lab.jpl.nasa.gov/JPL_Coding_Standard_C.pdf
[2]: https://web.cecs.pdx.edu/~kimchris/cs201/handouts/The%20Power%20of%2010%20-%20Rules%20for%20Developing%20Safety%20Critical%20Code.pdf