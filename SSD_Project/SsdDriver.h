
interface SsdDriver {
    virtual void write(unsigned int lba_index, unsigned int value) = 0;
    virtual unsigned int read(unsigned int lba_index) = 0;
};