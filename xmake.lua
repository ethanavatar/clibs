add_rules("mode.debug", "mode.release")
set_languages("c99", "c++11")

set_toolchains("clang")
set_toolset("cxx", "clang")
set_toolset("ld", "clang++")

set_optimize("fastest")

if is_mode("debug") then
    set_symbols("debug")
    set_optimize("none")
end
if is_mode("release") then
    set_symbols("hidden")
    set_optimize("fastest")
    set_strip("all")
end

package("unity")
    add_deps("cmake")
    set_sourcedir(path.join(os.scriptdir(), "unity_test"))
    on_install(function (package)
        local configs = { }
        local mode = package:debug() and "Debug" or "Release"
        local shared = package:config("shared") and "ON" or "OFF"
        table.insert(configs, "-DCMAKE_BUILD_TYPE=" .. mode)
        table.insert(configs, "-DBUILD_SHARED_LIBS=" .. shared)

        import("package.tools.cmake").install(package, configs)
    end)
package_end()
add_requires("unity")

target("test")
    set_kind("binary")

    add_includedirs("unity_test/src")
    add_packages("unity")

    add_includedirs("include")
    add_includedirs("test")
    add_files("test/test.c")

    set_configdir("$(buildir)/$(plat)/$(arch)/$(mode)/include")
    add_configfiles("include/*.h", { onlycopy = true })

