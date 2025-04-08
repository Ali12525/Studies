using ShopApp.Models;

namespace ShopApp.Core.DB
{
    public interface IDataBase
    {
        /// <summary>
        /// Returns a list of products from the database.
        /// </summary>
        List<Product> GetProducts();

        /// <summary>
        /// Inserts a new product
        /// </summary>
        void InsertProduct(Product product);

        /// <summary>
        /// Updates an existing product.
        /// </summary>
        void UpdateProduct(Product product);

        /// <summary>
        /// Deletes a product by its Id.
        /// </summary>
        void DeleteProduct(Guid id);
    }
}
