# SCRIPT TO GZIP CRITICAL FILES FOR ACCELERATED WEBSERVING
# see also https://community.platformio.org/t/question-esp32-compress-files-in-data-to-gzip-before-upload-possible-to-spiffs/6274/10

Import( 'env', 'projenv' )
import os
import gzip
import shutil
import glob

# HANDLE GZIP FILE
def gzip_file( src_path, dest_path ):
    with open( src_path, 'rb' ) as src, gzip.open( dest_path, 'wb' ) as destination:
        for chunk in iter( lambda: src.read(4096), b"" ):
            destination.write( chunk )

def gzip_webfiles(source, target, env):
    filetypes_to_gzip = ['html', 'css', 'js']
    exclude_files = ['svg']

    data_src_dir_path = os.path.join(env.get('PROJECT_DIR'), 'web')
    data_dir_path = os.path.join(env.get('PROJECT_DIR'), 'data')

    print(f'[PRE-BUILDING]: Compressing Web files in {data_src_dir_path}...')

    # Check if dir exists
    if(not os.path.exists(data_src_dir_path) ):
        print(f'[PRE-BUILDING - ERROR]: Source data dir {data_src_dir_path} not found!')
        return

    if(not os.path.exists(data_dir_path)):
        print(f'[PRE-BUILDING]: Creating data dir: {data_dir_path}')
        os.mkdir(data_dir_path)

    # Prepare a list of all files to compress
    files_to_gzip = []
    for extension in filetypes_to_gzip:        
        files_to_gzip.extend(glob.glob(os.path.join(data_src_dir_path, '*.' + extension)))

    all_files = glob.glob(os.path.join(data_src_dir_path, '*.*'))
    files_to_copy = list(set(all_files) - set(files_to_gzip) - set(exclude_files))

    for file in files_to_copy:
        # if os.path.exists(file):
        #     print(f'[PRE-BUILDING]: Removing previous version {file}')
        #     os.remove(file)

        print(f'[PRE-BUILDING]: Copying {file} to {data_dir_path}')
        shutil.copy(file, data_dir_path)

    was_error = False

    try:
        for source_file_path in files_to_gzip:
            base_file_path = source_file_path
            target_file_path = os.path.join(data_dir_path, os.path.basename( base_file_path ) + '.gz')

            if os.path.exists( target_file_path ):
                print(f'[PRE-BUILDING]: Removing previous version {target_file_path}')
                os.remove( target_file_path )

            print(f'[PRE-BUILDING]: Compressing file {source_file_path}')
            gzip_file(source_file_path, target_file_path)

    except IOError as e:
        was_error = True
        print(f'[PRE-BUILDING - ERROR]: Failure when compressing {source_file_path}! \nError: {e}.')
        return

    if was_error:
        print(f'[PRE-BUILDING - ERROR]: Some files could not be compressed!')

    else:
        print(f'[PRE-BUILDING]: Files successfully compressed.')

# IMPORTANT, this needs to be added to call the routine
env.AddPreAction('$BUILD_DIR/littlefs.bin', gzip_webfiles)            