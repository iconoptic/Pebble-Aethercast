<!--
source: https://developer.repebble.com/guides/app-resources/platform-specific/
fetched: 2026-09-07T17:42:33Z
-->

[pebble](/)

- [Tutorials](/tutorials/)
- [Get the SDK](/sdk/)
- [Guides](/guides/)
- [Documentation](/docs/)
- [Examples](/examples/)
- [Community](/community/)
- [Blog](https://repebble.com/blog)
- [More](/more/)

[Privacy](https://repebble.com/privacy/)\
[Cookies](https://repebble.com/terms)

[Publish](https://developer.rePebble.com/dashboard)

### [Guides](/guides/)

- [Table of Contents](/guides/toc/)
- [Alloy](/guides/alloy/)
- [App Resources](/guides/app-resources/)
  - [Animated Images](/guides/app-resources/animated-images/)
  - [App Assets](/guides/app-resources/app-assets/)
  - [Converting SVG to PDC](/guides/app-resources/converting-svg-to-pdc/)
  - [Fonts](/guides/app-resources/fonts/)
  - [Images](/guides/app-resources/images/)
  - [Pebble Draw Command File Format](/guides/app-resources/pdc-format/)
  - [Platform-specific Resources](/guides/app-resources/platform-specific/)
  - [Raw Data Files](/guides/app-resources/raw-data-files/)
  - [System Fonts](/guides/app-resources/system-fonts/)
- [Best Practices](/guides/best-practices/)
- [Communication](/guides/communication/)
- [Debugging](/guides/debugging/)
- [Design and Interaction](/guides/design-and-interaction/)
- [Events and Services](/guides/events-and-services/)
- [Graphics and Animations](/guides/graphics-and-animations/)
- [Pebble Packages](/guides/pebble-packages/)
- [Pebble Timeline](/guides/pebble-timeline/)
- [Tools and Resources](/guides/tools-and-resources/)
- [User Interfaces](/guides/user-interfaces/)

[](javascript:void(0);)

# Platform-specific Resources

You may want to use different versions of a resource on one or more of the Aplite, Basalt or Chalk platforms. To enable this, it is now possible to "tag" resource files with the attributes that make them relevant to a given platform.

The follows tags exist for each platform:

| Aplite  | Basalt     | Chalk      | Diorite | Emery      | Flint   |
|---------|------------|------------|---------|------------|---------|
| rect    | rect       | round      | rect    | rect       | rect    |
| bw      | color      | color      | bw      | color      | bw      |
| aplite  | basalt     | chalk      | diorite | emery      | flint   |
| 144w    | 144w       | 180w       | 144w    | 200w       | 144w    |
| 168h    | 168h       | 180h       | 168h    | 228h       | 168h    |
| compass | compass    | compass    |         | compass    | compass |
|         | mic        | mic        | mic     | mic        | mic     |
|         | strap      | strap      | strap   | strap      |         |
|         | strappower | strappower |         | strappower |         |
|         | health     | health     | health  | health     | health  |

To tag a resource, add the tags after the file's using tildes (`~`) — for instance, `example-image~color.png` to use the resource on only color platforms, or `example-image~color~round.png` to use the resource on only platforms with round, color displays. All tags must match for the file to be used. If no file matches for a platform, a compilation error will occur.

If the correct file for a platform is ambiguous, an error will occur at compile time. You cannot, for instance, have both `example~color.png` and `example~round.png`, because it is unclear which image to use when building for Chalk. Instead, use `example~color~rect.png` and `example~round.png`. If multiple images could match, the one with the most tags wins.

We recommend avoiding the platform specific tags (aplite, basalt etc). When we release new platforms in the future, you will need to create new files for that platform. However, if you use the descriptive tags we will automatically use them as appropriate. It is also worth noting that the platform tags are *not* special: if you have `example~basalt.png` and `example~rect.png`, that is ambiguous (they both match Basalt) and will cause a compilation error.

An example file structure is shown below.

my-project/ resources/ images/ example-image~bw.png example-image~color~rect.png example-image~color~round.png src/ main.c package.json wscript

This resource will appear in `package.json` as shown below.

"resources": { "media": \[ { "type": "bitmap", "name": "EXAMPLE_IMAGE", "file": "images/example-image.png" } \] }

**Single-platform Resources**

If you want to only include a resource on a **specific** platform, you can add a `targetPlatforms` field to the resource's entry in the `media` array in `package.json`. For example, the resource shown below will only be included for the Basalt build.

"resources": { "media": \[ { "type": "bitmap", "name": "BACKGROUND_IMAGE", "file": "images/background.png", "targetPlatforms": \[ "basalt" \] } \] }

You need JavaScript enabled to read and post comments.
