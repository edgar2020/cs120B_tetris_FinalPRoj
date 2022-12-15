template <typename T>
class queue
{
    unsigned char SZ;
    unsigned char numValues;
    T *values; 
 
public:
    queue(unsigned char s) 
    {
      SZ = s;
      numValues = 0;
      values = new T[SZ];
    };     // constructor
 
    T pop()
    {
      
      if(numValues > 0)
      {
        numValues--;
        T temp = values[0];
        for(char i = 0; i < SZ-1; i++)
          values[i] = values[i+1];
        values[SZ-1] = 0;
        
        return temp;
      }
      else
        return 0;
    }
    void push(T data)
    {
      values[numValues] = data;
      numValues++;
    }
    bool isEmpty()
    {
      for(char i = 0; i < numValues; i++)
      {
        if(values[i])
          return 0;
      }
      return 1;
    }


};