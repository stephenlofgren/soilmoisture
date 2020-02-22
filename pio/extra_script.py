Import("env")

def before_build(source, target, env):
    print("before_build")

def after_build(source, target, env):
    print("after_build")

def before_upload(source, target, env):
    print("before_upload")

def after_upload(source, target, env):
    print("after_upload")
    #env.Execute("python pio/buildscript_versioning.py")
    exec(open('pio/buildscript_versioning.py').read())
# after we upload we increment the version number
# platformio automatically builds so this is better on upload than build
env.AddPostAction("upload", after_upload)
# build the static files every time.
# building before_build the files still weren't there in time to be linked
exec(open("pio/build_static.py").read())
