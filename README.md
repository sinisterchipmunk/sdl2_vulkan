# `sdl2_vulkan`

Adds on to the [`ruby-sdl2`](https://github.com/ohai/ruby-sdl2) gem to expose
SDL2's Vulkan functions to Ruby in order to make it possible to present Vulkan
swapchain images to SDL2 windows.

Requires SDL v2.0.8 or higher, which is when Vulkan support was added to SDL.

## Installation

Add this line to your application's Gemfile:

```ruby
gem 'sdl2_vulkan'
```

And then execute:

    $ bundle

Or install it yourself as:

    $ gem install sdl2_vulkan

## Usage

See [sdl2_vulkan_test.rb#L11-L24](https://github.com/sinisterchipmunk/sdl2_vulkan/tree/master/test/sdl2_vulkan_test.rb#L11-L24) for usage.

## Development

After checking out the repo, run `bin/setup` to install dependencies. Then, run `rake test` to run the tests. You can also run `bin/console` for an interactive prompt that will allow you to experiment.

To install this gem onto your local machine, run `bundle exec rake install`. To release a new version, update the version number in `version.rb`, and then run `bundle exec rake release`, which will create a git tag for the version, push git commits and tags, and push the `.gem` file to [rubygems.org](https://rubygems.org).

## Contributing

Bug reports and pull requests are welcome on GitHub at https://github.com/sinisterchipmunk/sdl2_vulkan.

## License

The gem is available as open source under the terms of the [MIT License](https://opensource.org/licenses/MIT).
