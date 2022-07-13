# SCRIPT TO GZIP CRITICAL FILES FOR ACCELERATED WEBSERVING
# see also https://community.platformio.org/t/question-esp32-compress-files-in-data-to-gzip-before-upload-possible-to-spiffs/6274/10
#
# Ampliamente modificado  Ver original en el link citado anteriormente

Import( 'env', 'projenv' )
import os
import gzip
import shutil
import glob

# Funcion complementaria para utilizar gzip
def gzip_file( src_path, dst_path ):
    with open( src_path, 'rb' ) as src, gzip.open( dst_path, 'wb' ) as dst:
        for chunk in iter( lambda: src.read(4096), b"" ):
            dst.write( chunk )

# Comprime los archivos definidos de 'data_src/' en 'data/'
def gzip_webfiles( source, target, env ):
    # Tipos de archivos que necesitan ser comprimidos
    filetypes_to_gzip = [ 'css', 'html', 'js' ]
    print( '\nGZIP: Iniciando el proceso de gzipeado para la imágen SPIFFS...\n' )

    data_src_dir_path = os.path.join(env.get('PROJECT_DIR'), 'data_src')
    data_dir_path = env.get( 'PROJECTDATA_DIR' )

    

    # Verifica si existen data y datasrc. Si existe el primero y no el segundo lo renombra

    if(os.path.exists(data_dir_path) and not os.path.exists(data_src_dir_path) ):

        print('GZIP: El directorio "'+data_dir_path+'" existe, "'+data_src_dir_path+'" no se encuentra.')

        print('GZIP: Renombrando "' + data_dir_path + '" a "' + data_src_dir_path + '"')

        os.rename(data_dir_path, data_src_dir_path)

    # Elimina el directiorio 'data'

    if(os.path.exists(data_dir_path)):

        print('GZIP: Eliminando el directorio "data" ' + data_dir_path)

        shutil.rmtree(data_dir_path)

    # Recrea el directorio 'data' vacío

    print('GZIP: Re-creando un directorio de datos vacío ' + data_dir_path)

    os.mkdir(data_dir_path)

    # Determino los archivos a comprimir

    files_to_gzip = []

    for extension in filetypes_to_gzip:        

        files_to_gzip.extend( glob.glob( os.path.join( data_src_dir_path, '*.' + extension ) ) )

    

    all_files = glob.glob(os.path.join(data_src_dir_path, '*.*'))

    files_to_copy = list(set(all_files) - set(files_to_gzip))

    #print('MEWGZIP: archivos a copiar: ' + str(files_to_gzip))

    for file in files_to_copy:

        print('GZIP: Copiando archivo: ' + file + ' al directorio de datos')

        shutil.copy(file, data_dir_path)

    # Comprime y mueve los archivos

    #print('MEWGZIP: archivos a comprimir: ' + str(files_to_gzip))

    was_error = False

    try:

        for source_file_path in files_to_gzip:

            print( 'GZIP: Comprimiendo... ' + source_file_path )

            #base_file_path = source_file_path.replace( source_file_prefix, '' )

            base_file_path = source_file_path

            target_file_path = os.path.join( data_dir_path, os.path.basename( base_file_path ) + '.gz' )

            # print( 'GZIP: GZIPPING FILE...\n' + source_file_path + ' TO...\n' + target_file_path + "\n\n" )

            print( 'GZIP: Comprimido... ' + target_file_path )

            gzip_file( source_file_path, target_file_path )

    except IOError as e:
        was_error = True
        print( 'GZIP: Fallo al comprimir el archivo: ' + source_file_path )
        # print( 'GZIP: EXCEPTION... {}'.format( e ) )

    if was_error:
        print( 'GZIP: Fallo/Incomppleto.\n' )

    else:
        print( 'GZIP: Comprimido correctamente.\n' )

# IMPORTANT, this needs to be added to call the routine
env.AddPreAction( '$BUILD_DIR/littlefs.bin', gzip_webfiles )            