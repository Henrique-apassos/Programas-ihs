#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/uaccess.h> // copy_to_user, copy_from_user;

#define DEVICE_NAME "chardevice"
#define BUF_LEN 1024

static int major; // Número major do driver;
static char msg[BUF_LEN] = {0}; // Buffer interno do driver;
static short msg_size = 0;
static int device_open = 0;

// Funções à serem implementadas;
static int device_open_fn(struct inode *, struct file *);
static int device_release_fn(struct inode *, struct file *);
static ssize_t device_read_fn(struct file *, char *, size_t, loff_t *);
static ssize_t device_write_fn(struct file *, const char *, size_t, loff_t *);

// Estrutura de operações;
static struct file_operations fops = {
    .read = device_read_fn,
    .write = device_write_fn,
    .open = device_open_fn,
    .release = device_release_fn,
};

// Função chamada quando o módulo é carregado;
static int __init device_init(void) {
    major = register_chrdev(0, DEVICE_NAME, &fops);
    if (major < 0) {
        printk(KERN_ALERT "Falha ao registrar o driver.\n");
        return major;
    }
    printk(KERN_INFO "Driver carregado com sucesso. Major = %d\n", major);
    printk(KERN_INFO "Crie o device file com: mknod /dev/%s c %d 0\n", DEVICE_NAME, major);
    return 0;
}

// Função chamada quando o módulo é removido;
static void __exit device_exit(void) {
    unregister_chrdev(major, DEVICE_NAME);
    printk(KERN_INFO "Driver removido com sucesso.\n");
}

static int device_open_fn(struct inode *inode, struct file *file) {
    if(device_open) {// Verifica se o dispositivo ja esta aberto por outro processo
        printk(KERN_WARNING "chardevice: O dispositivo já está em uso.\n");
        return -EBUSY; // Retorna o erro "Device or resource busy"
    }

    device_open++; // Abre um dispositivo
    printk(KERN_INFO "chardevice: Dispositivo aberto com sucesso.\n");
    
    try_module_get(THIS_MODULE);
    /* Essa funcao eh usada aqui para invrementar a contagem de uso do modulo,
    previnindo que ele seja removido ('rmmod') enquanto estiver em uso*/
    return 0;
}

static int device_release_fn(struct inode *inode, struct file *file) {
    device_open--; // Decrementa o contador para indicar que o dispositivo esta livre

    module_put(THIS_MODULE);
    /*Decrementa a contagem de uso do modulo, agora se nenhum outro processo estiver
    usando o dispositivo, o modulo podera ser removido com 'rmmod'*/

    printk(KERN_INFO "chardevice: Dispositivo fechado com sucesso.\n");
    
    return 0;
}

static ssize_t device_read_fn(struct file *filp, char *buffer, size_t length, loff_t *offset) {
    int bytes_read = 0;

    if(*offset >= msg_size){ // Significa que o usuario ja leu tudo
        return 0;
    }

    bytes_read = msg_size - *offset; // Calcula quantos bytes podemos ler

    if(bytes_read > lenght){ // Limita a leitura ao tamanho do buffer do usuario
        bytes_read = lenght;
    }

    if(copy_to_user(buffer, msg + *offset, bytes_read) != 0){
        /* A funcao copy_to_user copia os dados do buffer do kernel (msg + *offset)
        para o buffer do usuario (buffer).
        A funcao retorna 0 em caso de sucesso, ou o numero de bytes que falharam ao copiar.*/
        printk(KERN_ALERT "chardevice: Erro ao copiar dados para o usuario (EFAULT). \n");
        return -EFAULT; // Retorna erro "Bad address"
    }

    *offset += bytes_read; // Atualiza o offset para a proxima leitura comecar de onde esta parou.

    printk(KERN_INFO "chardevice: %d bytes lidos do dispositivo.\n", bytes_read);

    return bytes_read; // Retorna o numero de bytes lidos com sucesso
}

static ssize_t device_write_fn(struct file *filp, const char *buffer, size_t length, loff_t *offset) {
    int bytes_to_write; // Determina quantos bytes vamos realmente esvrever

    if(length > BUF_LEN){ 
        /* Se o usuario tentar escrever mais do que o nosso buffer suporta,
        truncamos a escrita para o tamanho maximo do buffer */
        bytes_to_write = BUF_LEN;
        printk(KERN_WARNING "chardevice: Dados do usuario truncados para %d bytes.\n", BUF_LEN);
    }
    else
        bytes_to_write = length;
    
    if(copy_from_user(msg, buffer, bytes_to_write) != 0){
        /* A funcao copy_from_user copia os dados do buffer de usuario para o buffer do kernel (msg).
        A funcao retorna 0 em caso de sucesso, ou o numero de bytes que falharam ao ser copiados. */
        printk(KERN_ALERT "chardevice: Erro ao copiar dads do usuario (EFAULT).\n");
        return -EFAULT;
    }
    
    msg_size = bytes_to_write; // Atualiza o tamanho da mensagem interna

    if(msg_size < BUF_LEN){
        // Adiciona um terminador nulo por seguranca, caso os dados do usuario nao tenham.
        msg[msg_size] = '\0';
    }
    else
        msg[BUF_LEN - 1] = '\0' // Garante terminacao nula mesmo se o buffer estiver cheio.
    
    printk(KERN_INFO "chardevice: %d bytes recebidos do usuario. \n", msg_size);

    return msg_size; // Retorna o numero de bytes que foram escritos com sucesso.
}

module_init(device_init);
module_exit(device_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Monitores");
MODULE_DESCRIPTION("Driver de caractere simples: versão para completar");
