const binding = require("pkg-prebuilds")(
    __dirname,
    require("./binding-options")
);

// Copy exports so that we can customize them on the JS side without
// overwriting the binding itself.
Object.keys(binding).forEach(function (key) {
    module.exports[key] = binding[key];
});